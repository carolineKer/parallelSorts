#!/usr/bin/python
import sys

if len(sys.argv) != 3:
    print "Usage: %s data_file result_file"
    sys.exit(-1)

with open(sys.argv[1]) as data, open(sys.argv[2]) as res:
    data_list = []

    for line in data:
        if "process" in line:
            continue
        data_list.append(float(line))
    sorted_list = sorted(data_list)

    i = 0
    for line in res:
        if "process" in line:
            continue
        if float(line) != sorted_list[i]:
            print(str(sorted_list[i])+" expected, read " + line)
            print "KO"
            sys.exit(-1)
        i += 1

    print "OK"
