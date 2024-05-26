# compression-sorts
A benchmark stand for testing permute algorithms in terms of optimal data compression

## How To Install (Linux Ubuntu 22.04+)

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

2. You can check how good current clickhouse implementation works (it must supports **allow_experimental_optimized_row_order** setting). 

    If you want to do this, first of all, install and run clickhouse-server (for example you can use my [PR](https://github.com/ClickHouse/ClickHouse/pull/63578), use [this instruction](https://clickhouse.com/docs/en/development/developer-instruction)).

    After that just execute
    ```
    python3 clickhouse_tests/main.py
    ```

## How can you look at the test results?

### On your own hardware

First, run the benchmarks from the previous paragraph. After that, tables with tests results (in **.csv** format) will be located in the **tests_results** directory.

Fill free to use [research.ipynb](research.ipynb) jupyter notebook. 

If you just execute both benchmarks and after that execute **Run All**, the program will draw many graphs showing the current efficiency of the algorithms on various tests.

### On my hardware

You can also look at the results of tests performed on my hardware. 

They are in [research_with_results](https://github.com/ElderlyPassionFruit/compression-sorts/tree/research-with-results) branch, in the notebook **research.ipynb** and in the **tests_results** directory.

Characteristics of my hardware:

* Processor: 8-core Intel(R) Xeon(R) Platinum 8259CL CPU @ 2.50GHz
* RAM: 30GB 
* OS Version: Ubuntu 22.04.4 LTS
