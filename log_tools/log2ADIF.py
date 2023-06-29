#!/usr/bin/python3 /home/pi/sbitx/log2ADIF.py
# sbitx log to ADIF file
# Bob Benedict, KD8CGH, June 2023

import sqlite3

def find_band(freq): # assigns band names
    bds=""
    if (float(value) > 3500) and (float(value) < 4000):
        band="80M"
        band=f"{len(band)}>80M"
        bds = "<Band:" + band
        return bds
    if (float(value) > 7000) and (float(value) < 7500):
        band="40M"
        band=f"{len(band)}>40M"
        bds = "<Band:" + band
        return bds
    if (float(value) > 5330) and (float(value) < 5404):
        band="60M"
        band=f"{len(band)}>60M"
        bds = "<Band:" + band
        return bds
    if (float(value) > 10100) and (float(value) < 10150):
        band="30M"
        band=f"{len(band)}>30M"
        bds = "<Band:" + band    
        return bds
    if (float(value) > 14000) and (float(value) < 14350):
        band="20M"
        band=f"{len(band)}>20M"
        bds = "<Band:" + band
        return bds
    if (float(value) > 18068) and (float(value) < 18168):
        band="17M"
        band=f"{len(band)}>17M"
        bds = "<Band:" + band
        return bds
    if (float(value) > 21000) and (float(value) < 21450):
        band="15M"
        band=f"{len(band)}>15M"
        bds = "<Band:" + band
        return bds
    if (float(value) > 24890) and (float(value) < 24990):
        band="12M"
        band=f"{len(band)}>12M"
        bds = "<Band:" + band
        return bds
    if (float(value) > 28000) and (float(value) < 29700):
        band="10M"
        band=f"{len(band)}>10M"
        bds = "<Band:" + band
        return bds
#        print(bds)
    print("frequency error\n")
    bds="<Band:3>00M"  # band not found
    return


# Connect to the database, hardcode to default location
conn = sqlite3.connect('/home/pi/sbitx/data/sbitx.db')
cursor = conn.cursor()

# Read the table contents
cursor.execute("SELECT * FROM logbook")
data = cursor.fetchall()

names = ["ID","Mode","Freq","QSO_DATE","time_on","Operator","RST_sent","exch_sent","Call","RST_rcvd","exch_recv","tx_id","Comments"]

adif_output = "ADIF file\n"
adif_output += "created by sbitx\n"
adif_output += "ADIF version 3.1.4\n"  # not verified
adif_output += "<EOH>\n"

for row in data:
    adif_row = ""
    for i, value in enumerate(row):
        field_name = names[i]
        if i==2:
            band_str = find_band(float(value))  # assign band name
            adif_row += band_str
            value=float(value)/1000.   # change to MHz
        if i==3:
            value=(value.replace('-', ''))  # fix date format
        adif_row += f"<{field_name}:{len(str(value))}>{value} "
    adif_output += adif_row
    adif_output += "<EOR>\n"
print(adif_output)

# Write the ADIF output to a file
with open('/home/pi/sbitx/data/logADIF.adif', 'w') as file:
    file.write(adif_output)

# Close the database connection
cursor.close()
conn.close()