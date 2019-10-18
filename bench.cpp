#include <benchmark/benchmark.h>
#include <scn/scn.h>
#include <boost/spirit/home/x3.hpp>

#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <string>
#include <vector>

template <typename Int>
std::string generate_data(size_t n, bool comma) {
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<Int> int_dist(
        std::numeric_limits<Int>::min(), std::numeric_limits<Int>::max());

    std::ostringstream oss;
    for (size_t i = 0; i < n; ++i) {
        oss << int_dist(rng);
        if (comma) {
            oss << ',';
        }
        oss << ' ';
    }
    oss << int_dist(rng);
    return oss.str();
}

template <typename Int>
std::vector<std::string> generate_data_list(size_t n) {
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<Int> int_dist(
        std::numeric_limits<Int>::min(), std::numeric_limits<Int>::max());

    std::vector<std::string> vec;
    for (size_t i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << int_dist(rng);
        vec.push_back(oss.str());
    }
    return vec;
}

static void issue_spirit(benchmark::State& state) {
    using namespace boost::spirit;

    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), true);
        state.ResumeTiming();

        std::vector<int> read;
        bool r = x3::phrase_parse(data.begin(), data.end(),
                                  x3::int_ >> *(',' >> x3::int_),
                                  x3::ascii::space, read);
        if (!r) {
            state.SkipWithError("");
            break;
        }
    }
}
BENCHMARK(issue_spirit)->Arg(16)->Arg(64)->Arg(256);

static void issue_scn(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), true);
        auto view = scn::make_view(data);
        state.ResumeTiming();

        std::vector<int> read;
        while (true) {
            scn::string_view str;
            auto err = scn::getline(view, str, ',');
            if (!err) {
                if (err.error() == scn::error::end_of_range) {
                    break;
                }
                state.SkipWithError("");
                break;
            }

            int val{};
            auto tmp = scn::scan(str, scn::default_tag, val);
            if (!tmp) {
                state.SkipWithError("");
                break;
            }
            read.push_back(val);
        }
    }
}
BENCHMARK(issue_scn)->Arg(16)->Arg(64)->Arg(256);

static void issue_scn_list(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), true);
        state.ResumeTiming();

        std::vector<int> read;
        auto ret = scn::scan_list(scn::make_view(data), read, ',');
        if (!ret) {
            if (ret.error() != scn::error::end_of_range) {
                state.SkipWithError("");
            }
            break;
        }
    }
}
BENCHMARK(issue_scn_list)->Arg(16)->Arg(64)->Arg(256);

static void issue_istream(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), true);
        auto stream = std::istringstream{data};
        state.ResumeTiming();

        std::vector<int> read;
        while (true) {
            int tmp;
            if (!(stream >> tmp)) {
                state.SkipWithError("");
                break;
            }
            read.push_back(tmp);

            char ch;
            if (!(stream >> ch)) {
                if (stream.eof()) {
                    break;
                }
                state.SkipWithError("");
                break;
            }
        }
    }
}
BENCHMARK(issue_istream)->Arg(16)->Arg(64)->Arg(256);

static void nocomma_spirit(benchmark::State& state) {
    using namespace boost::spirit;

    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), false);
        state.ResumeTiming();

        std::vector<int> read;
        bool r = x3::phrase_parse(data.begin(), data.end(), *x3::int_,
                                  x3::ascii::space, read);
        if (!r) {
            state.SkipWithError("");
            break;
        }
    }
}
BENCHMARK(nocomma_spirit)->Arg(16)->Arg(64)->Arg(256);

static void nocomma_scn(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), false);
        auto view = scn::make_view(data);
        state.ResumeTiming();

        std::vector<int> read;
        while (true) {
            int val{};
            auto ret = scn::scan(view, scn::default_tag, val);
            if (!ret) {
                if (ret.error() != scn::error::end_of_range) {
                    state.SkipWithError(ret.error().msg());
                }
                break;
            }
            read.push_back(val);
        }
    }
}
BENCHMARK(nocomma_scn)->Arg(16)->Arg(64)->Arg(256);

static void nocomma_scn_list(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), false);
        state.ResumeTiming();

        std::vector<int> read;
        auto ret = scn::scan_list(scn::make_view(data), read, '\0');
        if (!ret) {
            if (ret.error() != scn::error::end_of_range) {
                state.SkipWithError(ret.error().msg());
            }
            break;
        }
    }
}
BENCHMARK(nocomma_scn_list)->Arg(16)->Arg(64)->Arg(256);

static void nocomma_istream(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto data =
            generate_data<int>(static_cast<size_t>(state.range(0)), false);
        auto stream = std::istringstream{data};
        state.ResumeTiming();

        std::vector<int> read;
        while (true) {
            int tmp;
            if (!(stream >> tmp)) {
                if (!stream.eof()) {
                    state.SkipWithError("");
                }
                break;
            }
            read.push_back(tmp);
        }
    }
}
BENCHMARK(nocomma_istream)->Arg(16)->Arg(64)->Arg(256);

static void single_spirit(benchmark::State& state) {
    using namespace boost::spirit;

    auto vec = generate_data_list<int>(8192);
    auto it = vec.begin();
    for (auto _ : state) {
        int i;
        bool r = x3::phrase_parse(it->begin(), it->end(), x3::int_,
                                  x3::ascii::space, i);
        if (!r) {
            state.SkipWithError("");
            break;
        }
        ++it;
        if (it == vec.end()) {
            it = vec.begin();
        }
    }
}
BENCHMARK(single_spirit);

static void single_scn(benchmark::State& state) {
    auto vec = generate_data_list<int>(8192);
    auto it = vec.begin();
    for (auto _ : state) {
        int i;
        auto ret = scn::scan(scn::make_view(*it), scn::default_tag, i);
        if (!ret) {
            state.SkipWithError("");
            break;
        }
        ++it;
        if (it == vec.end()) {
            it = vec.begin();
        }
    }
}
BENCHMARK(single_scn);

static void single_scn_integer(benchmark::State& state) {
    auto vec = generate_data_list<int>(8192);
    auto it = vec.begin();
    for (auto _ : state) {
        int i;
        auto ret = scn::parse_integer(scn::string_view{*it}, i);
        if (!ret) {
            state.SkipWithError("");
            break;
        }
        ++it;
        if (it == vec.end()) {
            it = vec.begin();
        }
    }
}
BENCHMARK(single_scn_integer);

static void single_istream(benchmark::State& state) {
    auto vec = generate_data_list<int>(8192);
    auto it = vec.begin();
    for (auto _ : state) {
        int i;
        if (!(std::istringstream{*it} >> i)) {
            state.SkipWithError("");
            break;
        }
        ++it;
        if (it == vec.end()) {
            it = vec.begin();
        }
    }
}
BENCHMARK(single_istream);

static void single_scanf(benchmark::State& state) {
    auto vec = generate_data_list<int>(8192);
    auto it = vec.begin();
    for (auto _ : state) {
        int i;
        sscanf(it->c_str(), "%d", &i);
        ++it;
        if (it == vec.end()) {
            it = vec.begin();
        }
    }
}
BENCHMARK(single_scanf);

BENCHMARK_MAIN();
