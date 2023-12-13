# Specify the path to your data file
file_path = 'tcpNewReno.txt'

# Read data from the file
with open(file_path, 'r') as file:
    data = file.read()

# Process the data and reverse values in the second column
lines = data.strip().split('\n')
col1=[]
col2=[]

for line in lines:
    parts = line.split()
    col1.append(int(parts[0]))
    col2.append(float(parts[1]))
    # reversed_data.append((int(parts[0]), float(parts[1])))

# Reverse the values in the second column
# reversed_data = [(item[0], item[1]) for item in reversed_data[::-1]]
col2.reverse()
# Write the reversed data back to the file
with open(file_path, 'w') as file:
    for i in range(len(col1)):
        file.write('{} {}\n'.format(col1[i], col2[i]))
