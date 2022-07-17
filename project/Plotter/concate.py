import csv
import time
import datetime


with open('MFC1.csv') as csv_file:
    for row in csv.reader(csv_file):
        print(row[0],time.mktime(datetime.datetime.strptime(row[0],"%d/%m/%Y %H:%M:%S").timetuple()),row[1])
