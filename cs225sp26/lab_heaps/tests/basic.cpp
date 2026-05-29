#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "../heap.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <time.h>
#include <utility>
#include <algorithm>

#include "catchlib.h"
#include "../random.h"

using namespace std;


bool isMinHeap(const std::vector<int>& vec) {
    size_t n = vec.size();
    for (size_t i = 0; i < n; ++i) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        if (left < n && vec[left] < vec[i]) {
            return false;
        }
        if (right < n && vec[right] < vec[i]) {
            return false;
        }
    }
    return true;
}


void helpTestHeapifyDown(const vector<int> & vals)
{
    heap<int> herp;
    for(size_t i = 0; i < vals.size(); ++i)
        herp.push(vals[i]);

    vector<int> sorted = vals;
    sort(sorted.begin(), sorted.end());

    for(size_t i = 0; i < sorted.size(); i++)
        REQUIRE(sorted[i] == herp.pop());
}

TEST_CASE("test_heapify_down_1")
{
    vector<int> vals = {6, 7, 8};
    helpTestHeapifyDown(vals);
}

TEST_CASE("test_heapify_down_2")
{
    vector<int> vals = {99, 7, 12, 945, 8, 1234};
    helpTestHeapifyDown(vals);
}

TEST_CASE("test_heapify_down_3")
{
    vector<int> vals = {30, 39, 69, 52, 64, 94, 98, 21, 25, 87, 1};
    helpTestHeapifyDown(vals);
}

TEST_CASE("test_heapify_down_4")
{
    vector<int> vals = {47, 36, 76, 40, 14, 100, 23, 56, 35, 62, 6, 16, 90, 37, 18};
    helpTestHeapifyDown(vals);
}

TEST_CASE("test_heapify_down_5")
{
    vector<int> vals = {16, 66, 7, 55, 29, 84, 64, 74, 82, 53, 10, 68, 62, 9,
            46, 67, 39, 25, 78, 48, 76, 19, 89, 22, 49, 83, 99, 45, 88, 28, 73,
            84, 7, 4, 30, 83, 48, 79, 54, 24, 92, 44, 97, 24, 46, 78, 28, 56,
            6, 74, 37, 82, 21, 71, 93, 74, 45, 15, 69, 35, 41, 85, 86, 62, 52, 71};
    helpTestHeapifyDown(vals);
}

TEST_CASE("test_build_heap_1")
{
	vector<int> vals = {3, 2, 1};
	heap<int> myHeap(vals);
	vector<int> test;
	myHeap.getElems(test);
	REQUIRE(isMinHeap(test) == true);
    sort(vals.begin(), vals.end());
	sort(test.begin(), test.end());
	REQUIRE(test == vals);
}

TEST_CASE("test_build_heap_2")
{
	vector<int> vals = {5, 7, 2, 9, 8, 1};
	heap<int> myHeap(vals);
	vector<int> test;
	myHeap.getElems(test);
	REQUIRE(isMinHeap(test) == true);
    sort(vals.begin(), vals.end());
	sort(test.begin(), test.end());
	REQUIRE(test == vals);
}

TEST_CASE("test_build_heap_3")
{
	vector<int> vals = {30, 39, 69, 11, 52, 64, 94, 98, 21, 25, 87, 1, 83};
	heap<int> myHeap(vals);
	vector<int> test;
	myHeap.getElems(test);
	REQUIRE(isMinHeap(test) == true);
    sort(vals.begin(), vals.end());
	sort(test.begin(), test.end());
	REQUIRE(test == vals);
}

TEST_CASE("test_build_heap_4")
{
	vector<int> vals = {47, 36, 76, 40, 14, 100, 23, 56, 70, 71, 35, 62, 6, 16, 90, 37, 18};
	heap<int> myHeap(vals);
	vector<int> test;
	myHeap.getElems(test);
	REQUIRE(isMinHeap(test) == true);
    sort(vals.begin(), vals.end());
	sort(test.begin(), test.end());
	REQUIRE(test == vals);
}

TEST_CASE("test_build_heap_5")
{
	vector<int> vals = {16, 66, 7, 55, 29, 84, 64, 74, 82, 53, 10, 68, 62, 9,
            46, 67, 39, 25, 78, 48, 76, 19, 89, 22, 49, 83, 99, 45, 88, 28, 73,
            84, 7, 4, 30, 83, 48, 79, 54, 24, 92, 44, 97, 24, 46, 78, 28, 56,
            6, 74, 37, 82, 21, 71, 93, 74, 45, 15, 69, 35, 41, 85, 86, 62, 52, 71};
	heap<int> myHeap(vals);
	vector<int> test;
	myHeap.getElems(test);
	REQUIRE(isMinHeap(test) == true);
    sort(vals.begin(), vals.end());
	sort(test.begin(), test.end());
	REQUIRE(test == vals);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    print_valgrind();

    int result = Catch::Session().run(argc, argv);

    return (result < 0xFF ? result : 0xFF);
}
