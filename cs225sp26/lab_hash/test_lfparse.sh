#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

if [ ! -f "./lfparse" ]; then
    echo "Error: lfparse executable not found."
    exit 1
fi

mkdir -p my_output

run_test() {
    local input_file=$1
    local soln_file=$2
    local base_name=$(basename "$input_file")
    
    # 使用独立的文件名，避免 log2 覆盖 log1 的结果
    local my_out="my_output/res_lfp_${base_name}.txt"
    local my_data="my_output/my_lfp_${base_name}.tmp"
    local soln_data="my_output/soln_lfp_${base_name}.tmp"

    echo -n "Testing LogfileParser with $base_name... "

    ./lfparse "$input_file" > "$my_out"

    # 提取数据、排序并去除行尾符干扰
    # 1. tail 去掉第一行
    # 2. sort 确保顺序一致（防止哈希遍历顺序问题）
    # 3. tr -d '\r' 删掉可能存在的 Windows 换行符
    tail -n +2 "$my_out" | sort | tr -d '\r' > "$my_data"
    tail -n +2 "soln/$soln_file" | sort | tr -d '\r' > "$soln_data"

    # 使用 -w 忽略所有空白字符进行终极对比
    if diff -q -w "$my_data" "$soln_data" > /dev/null; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Mismatch detected! Try this command to see the details:"
        echo "  diff -y \"$my_data\" \"$soln_data\""
    fi
}

echo "Starting tests for LogfileParser..."
echo "-------------------------------------------------------"

[ -f "data/log.txt" ] && run_test "data/log.txt" "soln_lfp_log.txt"
[ -f "data/log2.txt" ] && run_test "data/log2.txt" "soln_lfp_log2.txt"

echo "-------------------------------------------------------"
echo "Tests completed."

