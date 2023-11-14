# Python script to merge columns from three files
file1 = "sixth_NewReno.cwnd"
file2 = "sixth_Reno.cwnd"
file3 = "sixth.cwnd"
output_file = "mptcpNewReno.cwnd"

# Read data from files
data1 = {}
with open(file1, "r") as f:
    for line in f:
        cols = line.strip().split()
        data1[cols[0]] = float(cols[1])

# print(data1)
data2 = {}
with open(file2, "r") as f:
    for line in f:
        cols = line.strip().split()
        data2[cols[0]] = float(cols[1])

# print(data2)
data3 = {}
with open(file3, "r") as f:
    for line in f:
        cols = line.strip().split()
        data3[cols[0]] = float(cols[1])

# print(data3)
# Merge data and write to a new file
keys = list(data1.keys())
v1 = list(data1.values())
v2 = list(data1.values())
v3 = list(data1.values())

v_final=v1+v2+v3

data_final= {}

for i in range(len(keys)):
    data_final[keys[i]]=v_final[i]


# Write data_final to the output file
with open(output_file, "w") as out_file:
    for key, value in data_final.items():
        out_file.write(f"{key} {value}\n")

print(f"Data saved to {output_file}")

# print(data_final)


