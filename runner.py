import subprocess
import sys

# Accept user arguments from the command line
user_args = sys.argv[1:]

# Set the path to the Bazel executable
bazel_path = "bazel"

# Set the build target you want to invoke
build_target = ":fread_test"

# Invoke the Bazel build
# clean the build 
if "-c" in user_args:
    print("Clean Build")
    subprocess.call([bazel_path, "clean"])
# build
if "-b" in user_args:
    subprocess.call([bazel_path, "build", build_target])

# execute
if "-e" in user_args:
    subprocess.call(["./bazel-bin/fread_test"])