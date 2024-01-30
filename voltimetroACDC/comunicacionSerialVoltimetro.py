#!/usr/bin/python3
import csv
from time import time

import serial

# Your serial port might be different!
ser = serial.Serial('/tmp/ttyS1', timeout=1)

f = open("voltimetroACDC_log.csv", "a+")
writer = csv.writer(f, delimiter='-')

while True:
    s = ser.readline().decode()
    if s != "":
        rows = [x for x in s.split('-')]
        # Insert local time to list's first position
        rows.insert(0, int(time()))
        print(rows)
        writer.writerow(rows)
        f.flush()
