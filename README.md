# scnlib-spirit-benchmark

Benchmarking [scnlib](https://github.com/eliaskosunen/scnlib) against Boost.Spirit x3,
as prompted by [issue #9](https://github.com/eliaskosunen/scnlib/issues/9).

## Results

### Reading a list of comma-separated integers

List length | spirit | scnlib | scnlib (naive) | `<iostream>`
:---------- | -----: | -----: | -------------: | -----------:
16          | 1007   | 1445   | 1563           | 2177
64          | 1986   | 3913   | 4264           | 6736
256         | 5732   | 13242  | 15024          | 24907
