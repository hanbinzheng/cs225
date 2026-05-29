#!/bin/bash

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # 无颜色

if [ ! -f "./charcount" ]; then
    echo "Error: charcount executable not found."
    exit 1
fi

mkdir -p my_output

run_test() {
    local input_file=$1
    local threshold=$2
    local hash_type=$3
    local soln_file=$4
    local my_out="my_output/res_${hash_type}_$(basename "$input_file")_${threshold}.txt"
    
    # 临时文件，用于存储去掉第一行后的内容
    local my_data="my_output/my_data.tmp"
    local soln_data="my_output/soln_data.tmp"

    echo -n "Testing $hash_type with $(basename "$input_file")... "

    # 运行程序
    ./charcount "$input_file" "$threshold" "$hash_type" > "$my_out"

    # 【核心修改】：使用 tail -n +2 去掉第一行提示文字，只保留数据进行对比
    tail -n +2 "$my_out" > "$my_data"
    tail -n +2 "soln/$soln_file" > "$soln_data"

    # 对比数据部分
    if diff -q "$my_data" "$soln_data" > /dev/null; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Data mismatch! Check: diff $my_data $soln_data"
    fi
}

echo "Starting tests for charcount (ignoring header line)..."
echo "-----------------------------------"

run_test "data/aliceinwonderland.txt" 5000 "schash" "soln_cc_aliceinwonderland_5000.txt"
run_test "data/aliceinwonderland.txt" 5000 "lphash" "soln_cc_aliceinwonderland_5000.txt"

run_test "data/art_of_war.txt" 10000 "schash" "soln_cc_art_of_war_10000.txt"
run_test "data/art_of_war.txt" 10000 "lphash" "soln_cc_art_of_war_10000.txt"

echo "-----------------------------------"