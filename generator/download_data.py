# import requests
# import gzip
# import shutil

# def download_gzip_data(url, download_path, unpacked_path):
#     response = requests.get(url, stream=True)
#     if response.status_code == 200:
#         with open(download_path, 'wb') as f:
#             f.write(response.raw.read())

#         with gzip.open(download_path, 'rb') as f_in:
#             with open(unpacked_path, 'wb') as f_out:
#                 shutil.copyfileobj(f_in, f_out)

#         print("The file has been successfully downloaded and unzipped.")
#     else:
#         print(f"Error downloading file: status {response.status_code}")

# if __name__ == "__main__":
#     download_gzip_data("https://datasets.clickhouse.com/hits_compatible/hits.csv.gz", "hits.csv.gz", "hits.csv")
