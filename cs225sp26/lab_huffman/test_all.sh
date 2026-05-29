#!/bin/bash

mkdir -p test_output

passed=0
total=0

# iterate all files in data directory
for input_file in data/*.txt; do
    # get filename
    base_name=$(basename "$input_file" .txt)
    
    encoded_file="test_output/${base_name}.dat"
    tree_file="test_output/${base_name}.tree"
    decoded_file="test_output/${base_name}_decoded.txt"

    echo -n "Testing $input_file ... "
    ((total++))

    # encode
    ./encoder "$input_file" "$encoded_file" "$tree_file" > /dev/null 2>&1
    
    # decode
    ./decoder "$encoded_file" "$tree_file" "$decoded_file" > /dev/null 2>&1

    # diff
    if diff -q "$input_file" "$decoded_file" > /dev/null; then
        echo "PASS"
        ((passed++))
    else
        echo "FAIL (Files differ)"
    fi
done

echo "------------------------------------------------"
echo "Test Summary: $passed / $total passed."
if [ "$passed" -eq "$total" ]; then
    echo "Congratulations! Your HuffmanTree is rock solid."
    rm -r test_output
else
    echo "Some tests failed. Please check the logic in huffman_tree.cpp. And testing results are stored in the directory test_output."
fi
echo "------------------------------------------------"

