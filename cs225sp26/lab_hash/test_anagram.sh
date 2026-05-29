#!/bin/bash

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # 无颜色

# 检查可执行文件是否存在
if [ ! -f "./anagramtest" ]; then
    echo "Error: anagramtest executable not found. Please run 'make anagramtest' first."
    exit 1
fi

# 创建临时输出目录
mkdir -p my_output

# 定义测试函数
# 参数: 1:输入文件, 2:待查找单词, 3:哈希类型, 4:对应的标准答案文件
run_test() {
    local input_file=$1
    local word=$2
    local hash_type=$3
    local soln_file=$4
    local my_out="my_output/res_anagram_${hash_type}_${word}.txt"
    
    # 临时文件，用于存储去掉第一行后的纯数据
    local my_data="my_output/my_anagram_data.tmp"
    local soln_data="my_output/soln_anagram_data.tmp"

    echo -n "Testing $hash_type anagrams for '$word' using $(basename "$input_file")... "

    # 运行程序并重定向输出
    ./anagramtest "$input_file" "$word" "$hash_type" > "$my_out"

    # 使用 tail -n +2 去掉第一行提示文字 (Checking file... using XXXHashTable...)
    # 考虑到 anagram 输出顺序可能受哈希表实现影响，这里加入了 sort 保证对比的公平性
    tail -n +2 "$my_out" | sort > "$my_data"
    tail -n +2 "soln/$soln_file" | sort > "$soln_data"

    # 对比数据部分
    if diff -q "$my_data" "$soln_data" > /dev/null; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Data mismatch! Check differences with: diff $my_data $soln_data"
    fi
}

echo "Starting tests for AnagramFinder (ignoring header line)..."
echo "-------------------------------------------------------"

# 根据你 ls soln 的结果：soln_anagram_words_retinas.txt
# 测试用例：在 data/words.txt 中查找 retinas 的变位词
run_test "data/words.txt" "retinas" "schash" "soln_anagram_words_retinas.txt"
run_test "data/words.txt" "retinas" "lphash" "soln_anagram_words_retinas.txt"

echo "-------------------------------------------------------"
echo "Tests completed. Check 'my_output' folder if any failed."
