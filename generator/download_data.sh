#!/bin/bash

mkdir generator/downloads

# hits - main ClickHouse test dataset
curl -L https://datasets.clickhouse.com/hits_compatible/hits.csv.gz | gunzip > generator/downloads/hits.csv

# dictionary with english words
wget --output-document=generator/downloads/dictionary.csv https://www.bragitoff.com/wp-content/uploads/2016/03/dictionary.csv

# The UK property prices dataset
wget --output-document=generator/downloads/price_paid_transaction_data.csv http://prod.publicdata.landregistry.gov.uk.s3-website-eu-west-1.amazonaws.com/pp-complete.csv

# New York Public Library "What's on the Menu?" Dataset
wget --output-document=generator/downloads/2021_08_01_07_01_17_data.tgz https://s3.amazonaws.com/menusdata.nypl.org/gzips/2021_08_01_07_01_17_data.tgz

# md5sum 2021_08_01_07_01_17_data.tgz
# Checksum should be equal to: db6126724de939a5481e3160a2d67d15

tar xvf generator/downloads/2021_08_01_07_01_17_data.tgz -C generator/downloads

tail -n +2 generator/downloads/Dish.csv > tmp && mv tmp generator/downloads/Dish.csv
tail -n +2 generator/downloads/Menu.csv > tmp && mv tmp generator/downloads/Menu.csv
tail -n +2 generator/downloads/MenuItem.csv > tmp && mv tmp generator/downloads/MenuItem.csv
tail -n +2 generator/downloads/MenuPage.csv > tmp && mv tmp generator/downloads/MenuPage.csv
