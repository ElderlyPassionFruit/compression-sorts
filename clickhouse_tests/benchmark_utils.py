import clickhouse_connect
import pandas as pd

def create_client():
    client = clickhouse_connect.get_client(host='localhost', port=8123, user='default', password='', database='default')
    return client

def create_table(client, scheme, table_name, use_compression_optimization):
    sql_create_table = f"""
    CREATE TABLE IF NOT EXISTS {table_name} (
        {scheme}
    ) ENGINE = MergeTree()
    ORDER BY ()
    SETTINGS allow_experimental_improve_compression_rows_order={str(use_compression_optimization).lower()}
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
