import os

# Set the size of the file in bytes
file_size_mb = 100
file_size = file_size_mb*1000000
file_extension=".bin"
file_name="file"+str(file_size_mb)+file_extension

# binary file with garbage
# Open a file in binary write mode
# with open(file_name, 'wb') as f:
#     # Write 100MB of binary data to the file
#     f.write(os.urandom(file_size))

# binary file with repeated char
repeated_char = "A"

# Generate the binary file
with open(file_name, "wb") as f:
  # Write the repeated character to the file repeatedly
  # until the file reaches the specified size
  f.write(repeated_char.encode() * file_size)
