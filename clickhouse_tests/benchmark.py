from math import ceil
from benchmark_utils import *

def gen_batches(maxn = 100000, exponent = 1.2):
    batches = []
    current_size = 1
    while current_size <= maxn:
        batches.append(current_size)
        if (current_size == maxn):
            break
        current_size = ceil(current_size * exponent)
        if (current_size > maxn):
            current_size = maxn
    return batches

def add_name_suffix(create_table_query, suffix):
    elements = create_table_query.split('\n')
    elements[0] += suffix
    return '\n'.join(elements)

def test(table_name, batches):
    test_table_name = table_name + "_test"
    client = create_client()
    create_table_query = get_create_table_query(client, table_name)
    infos = []
    results = []

    for batch_size in batches:
        print(table_name, batch_size)
        batch_table_name = table_name + "_batch"
        batch_create_table_query = add_name_suffix(remove_order_by_from_query(create_table_query), "_batch")
        create_table_with_query(client, batch_table_name, batch_create_table_query)
        insert_random_data_part(client, table_name, batch_table_name, batch_size)

        batch_table_uncompressed_size = calc_uncompressed_size(client, batch_table_name)
        infos.append((table_name, str(batch_size), str(batch_table_uncompressed_size)))

        for use_initial_order_by in [True, False]:
            if use_initial_order_by:
                test_table_query = create_table_query
            else:
                test_table_query = remove_order_by_from_query(create_table_query)
            test_table_query = add_name_suffix(test_table_query, "_test")
            for use_compression_optimization in [False, True]:
                if not "SETTINGS" in test_table_query:
                    create_test_table_query = test_table_query + '\n' + f"SETTINGS allow_experimental_optimized_row_order={str(use_compression_optimization).lower()}"
                else:
                    create_test_table_query = test_table_query + ',\n' + f"allow_experimental_optimized_row_order={str(use_compression_optimization).lower()}"  
                create_table_with_query(client, test_table_name, create_test_table_query)
                elapsed_time = insert_random_data_part(client, batch_table_name, test_table_name, batch_size)
                table_size = calc_size(client, test_table_name)
                truncate_table(client, test_table_name)
                results.append((table_name, str(batch_size), str(table_size), str(elapsed_time), str(use_initial_order_by), str(use_compression_optimization)))

                drop_table(client, test_table_name)

        drop_table(client, batch_table_name)
    return infos, results

if __name__ == "__main__":
    client = create_client()

    N = 10**5
    # N = 10

    tests = []
    # total size 2*10**5
    tests.append(["supplier", gen_batches(N, 1.2)])
    # total size 3*10**6
    tests.append(["customer", gen_batches(N, 1.2)])
    # total size 600037902
    tests.append(["lineorder", gen_batches(N, 1.2)])
    # total size 1400000
    tests.append(["part", gen_batches(N, 1.2)])
    # total size 3000317
    tests.append(["trips", gen_batches(N, 1.2)])
    # total size 1329175
    tests.append(["menu_item_denorm", gen_batches(N, 1.2)])
    # total size 1329175
    tests.append(["hits_100m_obfuscated", gen_batches(N, 1.2)])

    infos = []
    results = []
    for [table_name, batches] in tests:
        i, r = test(table_name, batches)
        infos.extend(i)
        results.extend(r)
    # exit(0)
    kClickhouseInfosScheme = "name,rows,size"

    with open("tests_results/clickhouse_infos.csv", "w") as f:
        print(kClickhouseInfosScheme, file = f)
        for info in infos:
            line = ",".join(info)
            print(line, file = f)

    kClickhouseBenchmarksScheme = "name,rows,size,time,use_initial_order_by,use_compression_optimization"
    with open("tests_results/clickhouse_benchmarks.csv", "w") as f:
        print(kClickhouseBenchmarksScheme, file = f)
        for result in results:
            line = ",".join(result)
            print(line, file = f)
