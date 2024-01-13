#!/bin/bash

# Script to replace '00' with '01' based on input argument

# Check if the input argument is 1
if [ "$1" -eq 1 ]; then
    input_file="./tests/01_spec.json"
    input="./tests/01.in"
    output="./tests/01.out"
    answer="./tests/01.ans"
    number="4000"
else
    input_file="./tests/00_spec.json"
    input="./tests/00.in"
    output="./tests/00.out"
    answer="./tests/00.ans"
    number="200"
fi

# Navigate to the directory
cd ~/NTHU-OS-PThreads

# Run the script with the specified input and output files
./scripts/auto_gen_transformer --input $input_file --output transformer.cpp

# Clean and make
make clean
scl enable devtoolset-8 'make'

# Run the main program
./main number $input $output
# 原指令
# ./main 200 ./tests/00.in ./tests/00.out

# Verify the output
./scripts/verify --output $output --answer $answer
# ./scripts/verify --output ./tests/01.out --answer ./tests/01.ans
