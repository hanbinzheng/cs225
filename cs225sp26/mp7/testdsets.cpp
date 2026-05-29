/**
 * @file testdsets.cpp
 * Comprehensive tests of DisjointSets.
 * @date 2026
 */

#include <iostream>
#include <vector>
#include <cassert>
#include "dsets.h"

using std::cout;
using std::endl;
using std::vector;

/**
 * 简单的断言宏，输出详细的错误信息
 */
#define TEST_ASSERT(condition, msg) \
    do { \
        if (!(condition)) { \
            cout << "FAILED: " << msg << endl; \
            return false; \
        } \
    } while(0)

#define TEST_ASSERT_EQ(actual, expected, msg) \
    do { \
        if ((actual) != (expected)) { \
            cout << "FAILED: " << msg << endl; \
            cout << "  Expected: " << (expected) << endl; \
            cout << "  Actual:   " << (actual) << endl; \
            return false; \
        } \
    } while(0)

/**
 * 测试 1: 基本的 addelements 和 find 功能
 */
bool test_basic_addelements_and_find() {
    cout << "Test 1: Basic addelements and find..." << endl;
    
    DisjointSets s;
    s.addelements(10);
    
    // 最初每个元素都是自己的根
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQ(s.find(i), i, "Each element should be its own root initially");
    }
    
    // 测试 size
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQ(s.size(i), 1, "Each set should have size 1 initially");
    }
    
    return true;
}

/**
 * 测试 2: 基本的 union 操作
 */
bool test_basic_union() {
    cout << "Test 2: Basic union operations..." << endl;
    
    DisjointSets s;
    s.addelements(10);
    
    // 合并 0-1, 2-3, 4-5
    s.setunion(0, 1);
    s.setunion(2, 3);
    s.setunion(4, 5);
    
    // 检查同一个集合内的元素有相同的根
    TEST_ASSERT_EQ(s.find(0), s.find(1), "0 and 1 should be in same set");
    TEST_ASSERT_EQ(s.find(2), s.find(3), "2 and 3 should be in same set");
    TEST_ASSERT_EQ(s.find(4), s.find(5), "4 and 5 should be in same set");
    
    // 检查不同集合的根不同
    TEST_ASSERT(s.find(0) != s.find(2), "Sets {0,1} and {2,3} should be different");
    TEST_ASSERT(s.find(0) != s.find(4), "Sets {0,1} and {4,5} should be different");
    TEST_ASSERT(s.find(2) != s.find(4), "Sets {2,3} and {4,5} should be different");
    
    // 检查大小
    TEST_ASSERT_EQ(s.size(0), 2, "Set containing 0 should have size 2");
    TEST_ASSERT_EQ(s.size(2), 2, "Set containing 2 should have size 2");
    TEST_ASSERT_EQ(s.size(4), 2, "Set containing 4 should have size 2");
    
    return true;
}

/**
 * 测试 3: Union by size (按大小合并)
 */
bool test_union_by_size() {
    cout << "Test 3: Union by size verification..." << endl;
    
    DisjointSets s;
    s.addelements(10);
    
    // 创建大小为 3 的集合 {0,1,2}
    s.setunion(0, 1);
    s.setunion(0, 2);
    int root_small_set = s.find(0);
    
    // 创建大小为 2 的集合 {3,4}
    s.setunion(3, 4);
    
    // 合并大小 3 和大小 2 的集合，较大的（大小为3）应该是根
    s.setunion(0, 3);
    int new_root = s.find(0);
    
    // 较大的集合的根应该成为新根
    TEST_ASSERT_EQ(new_root, root_small_set, 
                   "Larger set's root should be the new root after union");
    
    // 检查合并后的大小
    TEST_ASSERT_EQ(s.size(0), 5, "Merged set should have size 5");
    
    // 所有元素应该在同一个集合中
    for (int i = 0; i <= 4; i++) {
        TEST_ASSERT_EQ(s.find(i), new_root, 
                       "All elements 0-4 should be in same set after union");
    }
    
    return true;
}

/**
 * 测试 4: 相同大小集合的合并（第二个指向第一个）
 */
bool test_union_same_size() {
    cout << "Test 4: Union of equal-sized sets..." << endl;
    
    DisjointSets s;
    s.addelements(6);
    
    // 创建两个大小都为 2 的集合
    s.setunion(0, 1);  // 集合 A: {0,1}
    s.setunion(2, 3);  // 集合 B: {2,3}
    
    int root_a = s.find(0);
    int root_b = s.find(2);
    
    // 合并相同大小的集合，第二个应该指向第一个
    s.setunion(0, 2);
    
    int new_root = s.find(0);
    
    // 根据规范：相同大小时，让第二个参数所在的树指向第一个参数所在的树
    TEST_ASSERT_EQ(new_root, root_a, 
                   "When equal size, second tree should point to first tree");
    
    // 合并后集合大小为 4
    TEST_ASSERT_EQ(s.size(0), 4, "Merged equal-sized sets should have size 4");
    
    return true;
}

/**
 * 测试 5: 路径压缩 (Path compression)
 */
bool test_path_compression() {
    cout << "Test 5: Path compression verification..." << endl;
    
    DisjointSets s;
    s.addelements(10);
    
    // 创建一个链状结构
    s.setunion(1, 0);
    s.setunion(2, 1);
    s.setunion(3, 2);
    s.setunion(4, 3);
    
    int root = s.find(0);
    
    // 在路径压缩之前，find(4) 可能深度较深
    // find 后，4 应该直接指向根
    TEST_ASSERT_EQ(s.find(4), root, "Element 4 should have root after find");
    
    // 多次调用 find 验证路径压缩
    for (int i = 0; i <= 4; i++) {
        TEST_ASSERT_EQ(s.find(i), root, 
                       "After find operations, all elements should point to root");
    }
    
    return true;
}

/**
 * 测试 6: 多次 addelements
 */
bool test_multiple_addelements() {
    cout << "Test 6: Multiple addelements calls..." << endl;
    
    DisjointSets s;
    s.addelements(5);  // 元素 0-4
    
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQ(s.find(i), i, "Initial elements should be their own root");
        TEST_ASSERT_EQ(s.size(i), 1, "Initial elements should have size 1");
    }
    
    s.addelements(3);  // 元素 5-7
    
    for (int i = 5; i < 8; i++) {
        TEST_ASSERT_EQ(s.find(i), i, "New elements should be their own root");
        TEST_ASSERT_EQ(s.size(i), 1, "New elements should have size 1");
    }
    
    // 验证新旧元素集合独立
    s.setunion(0, 1);
    s.setunion(5, 6);
    
    TEST_ASSERT(s.find(0) != s.find(5), "Old and new sets should be independent");
    
    TEST_ASSERT_EQ(s.size(0), 2, "Old set should have size 2");
    TEST_ASSERT_EQ(s.size(5), 2, "New set should have size 2");
    
    return true;
}

/**
 * 测试 7: 复杂合并操作
 */
bool test_complex_union_sequence() {
    cout << "Test 7: Complex union sequence..." << endl;
    
    DisjointSets s;
    s.addelements(20);
    
    // 创建多个集合
    // 集合 A: {0,1,2,3,4}
    for (int i = 1; i <= 4; i++) {
        s.setunion(0, i);
    }
    
    // 集合 B: {5,6,7,8}
    for (int i = 6; i <= 8; i++) {
        s.setunion(5, i);
    }
    
    // 集合 C: {9,10}
    s.setunion(9, 10);
    
    // 验证各个集合的大小
    TEST_ASSERT_EQ(s.size(0), 5, "Set A should have size 5");
    TEST_ASSERT_EQ(s.size(5), 4, "Set B should have size 4");
    TEST_ASSERT_EQ(s.size(9), 2, "Set C should have size 2");
    
    // 合并集合 A 和 C
    s.setunion(0, 9);
    TEST_ASSERT_EQ(s.size(0), 7, "Merging A and C should yield size 7");
    
    // 合并集合 B 到 A+C
    s.setunion(5, 0);
    TEST_ASSERT_EQ(s.size(0), 11, "Merging all should yield size 11");
    
    // 验证所有元素在同一个集合中
    int final_root = s.find(0);
    for (int i = 0; i <= 10; i++) {
        TEST_ASSERT_EQ(s.find(i), final_root, 
                       "All elements 0-10 should be in same set after all unions");
    }
    
    // 验证未合并的元素不在这个集合中
    TEST_ASSERT(s.find(11) != final_root, "Element 11 should not be in merged set");
    TEST_ASSERT_EQ(s.size(11), 1, "Element 11 should be singleton");
    
    return true;
}

/**
 * 测试 8: Union 时自动查找根（参数不必是根）
 */
bool test_union_with_non_roots() {
    cout << "Test 8: Union with non-root arguments..." << endl;
    
    DisjointSets s;
    s.addelements(10);
    
    // 创建一些合并
    s.setunion(0, 1);
    s.setunion(2, 3);
    s.setunion(0, 2);  // 现在 0,1,2,3 在一起
    
    // 使用非根元素调用 setunion
    s.setunion(1, 3);  // 1 和 3 都不是根
    
    // 它们应该还在同一个集合中，且没有破坏结构
    int root = s.find(0);
    TEST_ASSERT_EQ(s.find(1), root, "Element 1 should still be in same set");
    TEST_ASSERT_EQ(s.find(3), root, "Element 3 should still be in same set");
    TEST_ASSERT_EQ(s.size(1), 4, "Set size should still be 4");
    
    // 合并两个已经在同一个集合的元素不应该改变任何东西
    int size_before = s.size(0);
    s.setunion(0, 1);
    TEST_ASSERT_EQ(s.size(0), size_before, 
                   "Union of elements in same set should not change size");
    
    return true;
}

/**
 * 测试 9: size 函数的正确性
 */
bool test_size_correctness() {
    cout << "Test 9: Size function correctness..." << endl;
    
    DisjointSets s;
    s.addelements(15);
    
    // 测试单个元素的大小
    TEST_ASSERT_EQ(s.size(0), 1, "Singleton set should have size 1");
    TEST_ASSERT_EQ(s.size(7), 1, "Singleton set should have size 1");
    TEST_ASSERT_EQ(s.size(14), 1, "Singleton set should have size 1");
    
    // 创建不同大小的集合
    s.setunion(0, 1);                    // size 2
    s.setunion(0, 2);                    // size 3
    s.setunion(3, 4);                    // size 2
    s.setunion(5, 6);                    // size 2
    s.setunion(5, 7);                    // size 3
    s.setunion(0, 3);                    // size 3+2 = 5
    s.setunion(5, 0);                    // size 3+5 = 8
    
    TEST_ASSERT_EQ(s.size(0), 8, "After unions, set should have size 8");
    TEST_ASSERT_EQ(s.size(5), 8, "Set containing 5 should also have size 8");
    TEST_ASSERT_EQ(s.size(8), 1, "Element 8 should still be singleton");
    
    // 验证 size 返回的是集合大小，不是某个中间值
    int root = s.find(0);
    TEST_ASSERT_EQ(s.size(root), 8, 
                   "size on root should return correct set size");
    
    return true;
}

/**
 * 测试 10: 大规模操作
 */
bool test_large_scale() {
    cout << "Test 10: Large scale operations..." << endl;
    
    const int NUM_ELEMENTS = 1000;
    DisjointSets s;
    s.addelements(NUM_ELEMENTS);
    
    // 将所有偶数与下一个偶数合并
    for (int i = 0; i < NUM_ELEMENTS - 2; i += 2) {
        s.setunion(i, i + 2);
    }
    
    // 将所有奇数与下一个奇数合并
    for (int i = 1; i < NUM_ELEMENTS - 2; i += 2) {
        s.setunion(i, i + 2);
    }
    
    // 检查偶数都在一个集合中
    int even_root = s.find(0);
    for (int i = 2; i < NUM_ELEMENTS; i += 2) {
        TEST_ASSERT_EQ(s.find(i), even_root, 
                       "All even numbers should be in same set");
    }
    
    // 检查奇数都在另一个集合中
    int odd_root = s.find(1);
    for (int i = 3; i < NUM_ELEMENTS; i += 2) {
        TEST_ASSERT_EQ(s.find(i), odd_root, 
                       "All odd numbers should be in same set");
    }
    
    // 验证两个集合大小
    int num_evens = NUM_ELEMENTS / 2;
    int num_odds = NUM_ELEMENTS - num_evens;
    TEST_ASSERT_EQ(s.size(0), num_evens, "Even set should have correct size");
    TEST_ASSERT_EQ(s.size(1), num_odds, "Odd set should have correct size");
    
    // 合并偶数和奇数集合
    s.setunion(0, 1);
    TEST_ASSERT_EQ(s.size(0), NUM_ELEMENTS, 
                   "After merging, set should contain all elements");
    
    return true;
}

int main() {
    cout << "========== Running DisjointSets Comprehensive Tests ==========" << endl;
    cout << endl;
    
    // 运行所有测试
    vector<bool(*)()> tests = {
        test_basic_addelements_and_find,
        test_basic_union,
        test_union_by_size,
        test_union_same_size,
        test_path_compression,
        test_multiple_addelements,
        test_complex_union_sequence,
        test_union_with_non_roots,
        test_size_correctness,
        test_large_scale
    };
    
    const char* test_names[] = {
        "Basic addelements and find",
        "Basic union operations",
        "Union by size verification",
        "Union of equal-sized sets",
        "Path compression verification",
        "Multiple addelements calls",
        "Complex union sequence",
        "Union with non-root arguments",
        "Size function correctness",
        "Large scale operations"
    };
    
    int passed = 0;
    int failed = 0;
    
    for (size_t i = 0; i < tests.size(); i++) {
        cout << "----------------------------------------" << endl;
        if (tests[i]()) {
            cout << "✓ PASSED" << endl;
            passed++;
        } else {
            cout << "✗ FAILED" << endl;
            failed++;
        }
        cout << endl;
    }
    
    cout << "========================================" << endl;
    cout << "Results: " << passed << " passed, " << failed << " failed" << endl;
    
    if (failed == 0) {
        cout << "\n✅ SUCCESSFUL: All tests passed!" << endl;
        return 0;
    } else {
        cout << "\n❌ FAILED: " << failed << " test(s) failed!" << endl;
        return 1;
    }
}