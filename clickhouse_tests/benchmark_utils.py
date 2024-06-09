import time
import clickhouse_connect
import pandas as pd
from os import system

def create_client():
    client = clickhouse_connect.get_client(host='localhost', port=8123, user='default', password='', database='default')
    return client

def create_table_with_query(client, table_name, query):
    drop_table(client, table_name)
    client.command(query)
    print(f"Table created successfully. table: {table_name}")

def create_table(client, scheme, table_name, use_compression_optimization):
    sql_create_table = f"""
    CREATE TABLE IF NOT EXISTS {table_name} (
        {scheme}
    )
    ENGINE = MergeTree()
    ORDER BY ()
    SETTINGS allow_experimental_optimized_row_order={str(use_compression_optimization).lower()}
    """

    client.command(sql_create_table)

    print(f"Table created successfully. table: {table_name}")


def truncate_table(client, table_name):
    truncate_query = f"TRUNCATE TABLE IF EXISTS {table_name}"
    client.command(truncate_query)
    print(f"Truncated successfully. table: {table_name}")

def drop_table(client, table_name):
    drop_query = f"DROP TABLE IF EXISTS {table_name}"
    client.command(drop_query)
    print(f"Dropped successfully. table: {table_name}")

def load_data(file_path):
    with open(file_path, 'r') as f:
        return f.read()

def insert_data(client, table_name, data):
    insert_query = f"INSERT INTO {table_name} FORMAT CSV " + data
    client.command(insert_query)
    cnt_lines = len(data.split('\n'))
    print(f"Data inserted into '{table_name}' successfully. rows: {cnt_lines}")

def insert_random_data_part(client, table_from, table_to, batch_size):
    sql_query = f"""INSERT INTO {table_to} SELECT * FROM (SELECT * FROM {table_from} LIMIT {batch_size * 10}) ORDER BY rand() LIMIT {batch_size}"""
    start_time = time.time_ns()
    client.command(sql_query)
    end_time = time.time_ns()

    elapsed_time_ns = end_time - start_time

    print(f"Batch copied from: {table_from} to: {table_to}, size: {batch_size}, elapsed_time_ns: {elapsed_time_ns}")
    return elapsed_time_ns

def get_create_table_query(client, table_name):
    get_table_scheme_query = f"""
    SHOW CREATE TABLE {table_name};
    """

    query = client.command(get_table_scheme_query)

    return "\n".join(query.replace('\\\'', '\'').split('\\n'))

def remove_order_by_from_query(query):
    elements = query.split('\n')
    new_elements = []
    for i in elements:
        if "PRIMARY KEY" in i:
            continue
        if "ORDER BY" in i:
            continue
        if "SAMPLE BY" in i:
            continue
        new_elements.append(i)
        if "ENGINE" in i:
            new_elements.append("ORDER BY ()")
    return '\n'.join(new_elements)

def calc_size(client, table_name):
    calc_size_query = f"""
    SELECT sum(bytes_on_disk) AS total_size_in_bytes
    FROM system.parts 
    WHERE table = '{table_name}'
    AND active
    """

    result = client.command(calc_size_query)

    print(f"Size calculated successfully. table: {table_name}, {result} bytes")
    return result

def calc_uncompressed_size(client, table_name):
    calc_uncompressed_size_query = f"""
    SELECT sum(data_uncompressed_bytes) AS uncompressed_size
    FROM system.parts
    WHERE table = '{table_name}' AND active
    """
    result = client.command(calc_uncompressed_size_query)
    print(f"Uncompressed size calculated successfully. table: {table_name}, {result} bytes")
    return result    
