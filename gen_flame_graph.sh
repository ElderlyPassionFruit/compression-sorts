#!/bin/bash

sudo perf record -F 99 -g ./bin/benchmark

sudo perf script > out.perf

./../FlameGraph/stackcollapse-perf.pl out.perf > out.folded

./../FlameGraph/flamegraph.pl out.folded > flamegraph.svg

rm -f perf.data*
rm -f out.pref
rm -f out.folded
