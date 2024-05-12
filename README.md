# compression-sorts
A benchmark stand for testing permute algorithms in terms of optimal data compression

## How To Install (Linux)

First of all download repository:

```
git clone https://github.com/ElderlyPassionFruit/compression-sorts.git
```

After that initialize everything.

P.S. There and then, all commands must be executed strictly in the compression-sorts directory.

```
cd compression-sorts
./setup.sh
```

Yoo-hoo! Now you are ready to use **compression-sorts**!

## Benchmarks

There are two benchmarks in project.

1. You can use my native implementation of all algorithms, just execute
    ```
    ./bin/benchmark
    ```

2. You can check how good current clickhouse implementation works (it must supports **allow_experimental_improve_compression_rows_order** setting). 

    If you want to do this, first of all, install and run clickhouse-server (for example you can use my [PR](https://github.com/ClickHouse/ClickHouse/pull/63578), use [this instruction](https://clickhouse.com/docs/en/development/developer-instruction)).

    After that just execute
    ```
    python3 clickhouse_tests/main.py
    ```

## How can you look at the test results?

Fill free to use [research.ipynb](research.ipynb) jupyter notebook. 

If you just execute both benchmarks and after that execute **Run All**, the program will draw many graphs showing the current efficiency of the algorithms on various tests.
