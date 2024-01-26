#!/usr/bin/env python3
import time

file_path = '/home/sdp/random_strings.txt'

with open(file_path, 'r') as file:
    random_strings_read = [line.rstrip() for line in file.readlines()]

start_time = time.time()
s = set()
for i in range(len(random_strings_read)):
    s.add(random_strings_read[i])
end_time = time.time()
execution_time = end_time - start_time

print(f"execution time: {execution_time} s")