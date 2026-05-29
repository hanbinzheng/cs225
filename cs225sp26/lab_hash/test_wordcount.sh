#!/bin/bash

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # 无颜色

# 检查可执行文件是否存在
if [ ! -f "./wordcount" ]; then
    echo "Error: wordcount executable not found. Please run 'make wordcount' first."
    exit 1
fi

# 创建临时输出目录
mkdir -p my_output

# 定义测试函数
# 参数: 1:输入文件, 2:频率阈值, 3:哈希类型, 4:对应的标准答案文件
run_test() {
    local input_file=$1
    local threshold=$2
    local hash_type=$3
    local soln_file=$4
    local base_name=$(basename "$input_file")
    local my_out="my_output/res_wc_${hash_type}_${base_name}_${threshold}.txt"
    
    # 临时文件，用于存储去掉第一行后的纯数据
    local my_data="my_output/my_wc_data.tmp"
    local soln_data="my_output/soln_wc_data.tmp"

    echo -n "Testing $hash_type with $base_name (threshold: $threshold)... "

    # 运行程序并重定向输出
    ./wordcount "$input_file" "$threshold" "$hash_type" > "$my_out"

    # 使用 tail -n +2 去掉第一行提示文字 (Finding words in...)
    tail -n +2 "$my_out" > "$my_data"
    tail -n +2 "soln/$soln_file" > "$soln_data"

    # 对比数据部分
    if diff -q "$my_data" "$soln_data" > /dev/null; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Data mismatch! Check differences with: diff $my_data $soln_data"
    fi
}

echo "Starting tests for wordcount (ignoring header line)..."
echo "-------------------------------------------------------"

# 根据你 ls soln 的结果匹配测试用例 (soln_wc_...)

# 测试用例 1: Alice in Wonderland (threshold 200)
run_test "data/aliceinwonderland.txt" 200 "schash" "soln_wc_aliceinwonderland_200.txt"
run_test "data/aliceinwonderland.txt" 200 "lphash" "soln_wc_aliceinwonderland_200.txt"

# 测试用例 2: Metamorphoses (threshold 1000)
run_test "data/metamorphoses.txt" 1000 "schash" "soln_wc_metamorphoses_1000.txt"
run_test "data/metamorphoses.txt" 1000 "lphash" "soln_wc_metamorphoses_1000.txt"

# 测试用例 3: War and Peace (threshold 5000)
run_test "data/war-and-peace.txt" 5000 "schash" "soln_wc_war-and-peace_5000.txt"
run_test "data/war-and-peace.txt" 5000 "lphash" "soln_wc_war-and-peace_5000.txt"

echo "-------------------------------------------------------"
echo "Tests completed. Raw outputs are in 'my_output' folder."