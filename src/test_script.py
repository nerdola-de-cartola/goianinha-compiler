import subprocess
import os
import sys

tests_path = "tests"
compiler_path = "build/compiler"

for folder in os.listdir(tests_path):
    print(f"Testing folder {folder}")

    files = os.listdir(tests_path + "/" + folder)
    for file in files:
        print(f"Testing file {file}")
        #exec(compiler_path)

        process = subprocess.Popen(
            [compiler_path, f"tests/{folder}/{file}"],
            stdout=sys.stdout,
            stderr=sys.stderr,
            text=True
        )

        process.wait()
