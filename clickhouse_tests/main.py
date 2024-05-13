from benchmark_utils import *
from os import listdir
from pathlib import Path

def test(client, table_name, directory, scheme):
    files = listdir(directory)
    files = sorted(files, key = lambda x: int(x.split('.')[0]))
    files = files[:5]
    results = []
    for use_compression_optimization in [False, True]:
        drop_table(client, table_name)
        create_table(client, scheme, table_name, use_compression_optimization)
        for file in files:
            print(file)
            data = load_data(directory / file)
            insert_data(client, table_name, data)
            size = calc_size(client, table_name)
            results.append((str(directory / file), file.split('.')[0], str(len(scheme.split('\n'))), str(size), str(use_compression_optimization)))
            truncate_table(client, table_name)
    return results

def test_with_equal_type(directory, typename, cnt_columns):
    client = create_client()
    scheme = ',\n'.join([f"column_{i} {typename}" for i in range(0, cnt_columns)]) + '\n'
    return test(client, "test_table", directory, scheme)

def test_with_scheme(directory, scheme):
    client = create_client()
    return test(client, "test_table", directory, scheme)

if __name__ == '__main__':
    results = []
    results.extend(test_with_equal_type(Path("tests_data/int/random_small"), "Int8", 1))
    results.extend(test_with_equal_type(Path("tests_data/int/random_big"), "Int64", 1))
    results.extend(test_with_equal_type(Path("tests_data/int/many_random_small"), "Int8", 20))
    results.extend(test_with_equal_type(Path("tests_data/english/dictionary"), "String", 3))
    results.extend(test_with_equal_type(Path("tests_data/clickhouse/price_paid_transaction_data"), "String", 16))
    results.extend(test_with_equal_type(Path("tests_data/clickhouse/dish"), "String", 9))
    results.extend(test_with_equal_type(Path("tests_data/clickhouse/menu"), "String", 20))
    results.extend(test_with_equal_type(Path("tests_data/clickhouse/menu_item"), "String", 9))
    results.extend(test_with_equal_type(Path("tests_data/clickhouse/menu_page"), "String", 7))
    results.extend(test_with_equal_type(Path("tests_data/clickhouse/hits"), "String", 105))

    kClickhouseTestScheme = "name,raws,columns,size,use_compression_optimization"

    with open("tests_results/clickhouse_tests.csv", "w") as f:
        print(kClickhouseTestScheme, file = f)
        for result in results:
            line = ",".join(result)
            print(line, file = f)
