# scnlib-spirit-benchmark

Benchmarking [scnlib](https://github.com/eliaskosunen/scnlib) against Boost.Spirit x3,
as prompted by [issue #9](https://github.com/eliaskosunen/scnlib/issues/9).

## Results

### Reading a single integer

spirit::x3 | `scn::scan` | `scn::parse_integer` | `<iostream>` | `sscanf`
---------: | ----------: | -------------------: | -----------: | -------:
14         | 44          | 18                   | 261          | 94

### Reading a list of whitespace-separated integers

List length | spirit::x3 | scnlib | scnlib (naive) | `<iostream>`
:---------- | ---------: | -----: | -------------: | -----------:
16          | 905        | 1432   | 1447           | 1901
64          | 1732       | 3784   | 3826           | 5465
256         | 4950       | 12888  | 13101          | 19438

### Reading a list of comma-separated integers

List length | spirit::x3 | scnlib | scnlib (naive) | `<iostream>`
:---------- | ---------: | -----: | -------------: | -----------:
16          | 940        | 1418   | 1486           | 2026
64          | 1785       | 3780   | 4022           | 5917
256         | 5026       | 12993  | 13882          | 21295
