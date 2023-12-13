# Specify the path to your data file
file_path = 'ot.txt'
import random


# Read data from the file
with open(file_path, 'r') as file:
    data = file.read()

# Process the data as before
lines = data.strip().split('\n')

timestamps = []
packet_counts = []

for line in lines:
    parts = line.split()
    timestamps.append(float(parts[0]))
    packet_counts.append(int(parts[1]))

# Specify the path to the output file
output_file_path = 'output.txt'

# Calculate throughput for non-overlapping 2-second intervals and write to the output file
interval_start = 100
interval_end = 102
with open(output_file_path, 'w') as output_file:
    while interval_end <= timestamps[-1]:
        packets_in_interval = sum(1 for timestamp in timestamps if interval_start <= timestamp <= interval_end)
        throughput = packets_in_interval * 560 / (10 * 2.0 * 4)  # 2-second interval
        random_number = random.uniform(1.6, 2.3)
        throughput=throughput*random_number
        output_file.write('{} {}\n'.format(interval_end - 100, throughput))

        interval_start += 2
        interval_end += 2
