# scnlib-spirit-benchmark

Benchmarking [scnlib](https://github.com/eliaskosunen/scnlib) against Boost.Spirit x3,
as prompted by [issue #9](https://github.com/eliaskosunen/scnlib/issues/9).

Requires the `dev`-branch of scnlib.

## Results

### spirit-oriented benchmark

List length | spirit | scnlib | % difference
:---------- | -----: | -----: | -----------:
16          | 7473   | 8791   | 18
64          | 26990  | 31683  | 17
256         | 105858 | 123072 | 16
