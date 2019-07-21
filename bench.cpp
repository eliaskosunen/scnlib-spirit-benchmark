#include <benchmark/benchmark.h>
#include <scn/scn.h>
#include <boost/spirit/home/x3.hpp>

#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <string>
#include <vector>

template <typename Float>
std::string generate_data(size_t n)
{
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<int> int_dist(-8, 16);
    std::uniform_real_distribution<Float> float_dist(Float(0.0), Float(1.0));

    std::ostringstream oss;
    for (size_t i = 0; i < n; ++i) {
        auto f = float_dist(rng);
        auto exp = int_dist(rng);
        f = std::scalbn(f, exp);
        oss << f << ", ";
    }
    oss << float_dist(rng);
    return oss.str();
}

static void issue_spirit(benchmark::State& state)
{
    using namespace boost::spirit;

    for (auto _ : state) {
        auto data = generate_data<double>(static_cast<size_t>(state.range(0)));

        std::vector<double> read;
        bool r = x3::phrase_parse(data.begin(), data.end(),
                                  x3::double_ >> *(',' >> x3::double_),
                                  x3::ascii::space, read);
        if (!r) {
            state.SkipWithError("");
            break;
        }
    }
}
BENCHMARK(issue_spirit)->Arg(16)->Arg(64)->Arg(256);

static void issue_scn(benchmark::State& state)
{
    for (auto _ : state) {
        auto data = generate_data<double>(static_cast<size_t>(state.range(0)));

        std::vector<double> read;
        auto stream = scn::make_stream(data);
        while (true) {
            std::string str;
            auto err = scn::getline(stream, str, ',');
            if (!err) {
                if (err == scn::error::end_of_stream) {
                    break;
                }
                state.SkipWithError("");
                break;
            }

            auto strstream = scn::make_stream(str);
            auto tmp = scn::get_value<double>(strstream);
            if (!tmp) {
                state.SkipWithError("");
                break;
            }
            read.push_back(tmp.value());
        }
    }
}
BENCHMARK(issue_scn)->Arg(16)->Arg(64)->Arg(256);

BENCHMARK_MAIN();
