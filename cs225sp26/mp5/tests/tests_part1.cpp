#include "../cs225/catch/catch.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <locale>

#include "../kdtree.h"
#include "../point.h"

// https://stackoverflow.com/a/217605
// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// compare the two streams for equality
bool tree_equals_output(stringstream & s, string filename)
{
    ifstream file(filename);
    string soln_s;
    string out_s;

    while(getline(file, soln_s))
    {
        if(!getline(s, out_s))
            return false;

        rtrim(soln_s);
        rtrim(out_s);

        if(out_s != soln_s)
            return false;
    }
    if(getline(s, soln_s))
        return false;

    return true;
}

void compareBinaryFiles( string yourFile, string ourFile ) 
{
    ifstream ourBinary( ourFile, ios::binary );
    stringstream ours;
    ours << ourBinary.rdbuf();

    if(!tree_equals_output(ours, yourFile))
        FAIL( "Your output in \"" + yourFile + "\" does not match our output in \"" + ourFile + "\"");

    REQUIRE( true );
}

TEST_CASE("KDTree::testSmallerDimVal Tests", "[weight=1][part=1]") {
  vector<Point<3>> pts;
  KDTree<3> tree(pts);
  Point<3> a(1, 2, 3);
  Point<3> b(3, 2, 1);

  REQUIRE( tree.smallerDimVal(a, b, 0) == true );
  REQUIRE( tree.smallerDimVal(a, b, 2) == false );
  REQUIRE( tree.smallerDimVal(a, b, 1) == true );   // based on operator<
}

TEST_CASE("KDTree::shouldReplace Tests", "[weight=1][part=1]") {
  vector<Point<3>> pts;
  KDTree<3> tree(pts);

  Point<3> target(1, 3, 5);
  Point<3> currentBest1(1, 3, 2);
  Point<3> possibleBest1(2, 4, 4);
  Point<3> currentBest2(1, 3, 6);
  Point<3> possibleBest2(2, 4, 4);
  Point<3> currentBest3(0, 2, 4);
  Point<3> possibleBest3(2, 4, 6);

  REQUIRE( tree.shouldReplace(target, currentBest1, possibleBest1) == true );
  REQUIRE( tree.shouldReplace(target, currentBest2, possibleBest2) == false );
  REQUIRE( tree.shouldReplace(target, currentBest3, possibleBest3) == false );  // operator<
}


//
// Simple Constructor
//
template <int K>
void _test_linear_constructor(int size) {
  vector<Point<K>> points;
  points.reserve(size);
  for (int i = 0; i < size; i++) {
      Point<K> p;
      for (int j = 0; j < K; j++)
          p[j] = i;
      points.push_back(p);
  }

  KDTree<K> tree(points);

  std::string fname = "test_result_kdtree_"+to_string(size)+".kd";

  ofstream outputFile(fname, ofstream::out);
  tree.printTree(outputFile, colored_out::DISABLE, -1);
  outputFile.close();

  compareBinaryFiles(fname, "tests/expected_kdtree_"+to_string(size)+".kd" );
}

TEST_CASE("KDTree constructor, 1D (Dim=1)", "[weight=1][part=1]") {
  _test_linear_constructor<1>(10);
}

TEST_CASE("KDTree constructor, 3D (Dim = 3)", "[weight=1][part=1]") {
  _test_linear_constructor<3>(31);
  }


//
// Simple Nearest Neighbor
//
template <int K>
void _test_linear_nearestNeighbor(int size) {
  vector<Point<K>> points;
  points.reserve(size);
  for (int i = 0; i < size; i++) {
      Point<K> p;
      for (int j = 0; j < K; j++)
          p[j] = i;
      points.push_back(p);
  }

  KDTree<K> tree(points);
  for (int i = 0; i < size; i++) {
    REQUIRE ( tree.findNearestNeighbor(points[i]) == points[i] );
  }
}

TEST_CASE("KDTree::findNearestNeighbor, exact match, 1D (Dim=1)", "[weight=1][part=1]") {
  _test_linear_nearestNeighbor<1>(10);
}

TEST_CASE("KDTree::findNearestNeighbor, exact match, 3D (Dim=3)", "[weight=1][part=1]") {
  _test_linear_nearestNeighbor<3>(31);
  }


/*****************************************
*                    8                   *
*     X              7     X             *
*                    6                   *
*                    5                   *
*                    4                   *
*                    3                   *
*                    2             X     *
*       O            1                   *
* 9876543210987 X 32101234567890123456789*
*       X            1                   *
*                    2                   *
*                    3             X     *
*                    4                   *
*                    5                   *
*                    6                   *
*                    7                   *
*                    8                   *
*****************************************/
// Looking for O, Xs are points
TEST_CASE("KDTree::findNearestNeighbor (2D), returns correct result", "[weight=1][part=1]") {
  double coords[6][2] = {
    {-15, 7}, {6, 7}, {-13, -1},
    {-5, 0}, {14, -3}, {14, 2}
  };
  double targetCoords[2] = {-13, 1};
  double expectedCoords[2] = {-13, -1};

  vector<Point<2>> points;
  for (int i = 0; i < 6; ++i)
      points.push_back(Point<2>(coords[i]));
  Point<2> target(targetCoords);
  Point<2> expected(expectedCoords);

  KDTree<2> tree(points);
  REQUIRE( tree.findNearestNeighbor(target) == expected );
}


/*
 * Extend Point to create a MinePoint, having an additional field for if the
 * current point is a mine.  If a mine is visited, the test case FAILs.
 */
template <int Dim>
class MineActionFAIL : public Point<Dim>::MineAction {
  public:
    void onMine(const Point<Dim> & point) const {
        // only fail if trigger is set;
        // gets set after construction and before findNN call
        if (trigger)
            FAIL("The point " << point << " should not be visited during this query.");
    }

    bool trigger = false;
};


/***********************
*    X      8          *
*           7          *
*       X   6          *
*     O  X      5      *
*      N    X   4      *
*               3      *
*           2          *
*           1          *
* 098765432101 X 567890*
*           1          *
*           2          *
*           3     M    *
*           4          *
*           5   X      *
*           6      M   *
*           7 M        *
***********************/
// M: mine
// O: search target
// N: nearest neighbor
// X: non-mine elements
TEST_CASE("KDTree::findNearestNeighbor (2D), testing correct path", "[weight=1][part=1]") {
    double coords[10][2] = {
      {-8, 7}, {-6, 4}, {-5, 6}, {-3, 5}, {0, 7},
      //                ^NN
      {2, -7}, {3, 0},  {5, -4}, {6, -3}, {7, -6}
      //^M     ^M       ^M       ^M       ^M
    };

    bool isMine[10] = {false, false, false, false, false, true, true, true, true, true};
    double targetCoords[2] = {-7, 5};
    double expectedCoords[2] = {-6, 4};

    // Using MinePoint to FAIL for bad paths
    vector<Point<2>> points;
    MineActionFAIL<2> action;
    for (int i = 0; i < 10; ++i)
        points.push_back(Point<2>(coords[i], isMine[i], &action));
    Point<2> target(targetCoords);
    Point<2> expected(expectedCoords);

    KDTree<2> tree(points);

    stringstream s;
    tree.printTree(s, colored_out::enable_t::DISABLE);
    INFO(s.str());

    action.trigger = true;

    REQUIRE( tree.findNearestNeighbor(target) == expected );
}


/*

*/
TEST_CASE("KDTree::findNearestNeighbor (2D), testing correct path with fence jumping", "[weight=1][part=1]") {
  double coords[20][2] = {{84, 44},  // mine
                          {74, 0},   // mine
                          {54, 62},  // mine
                          {59, 0},   // mine
                          {34, 15},  // mine
                          {42, 63},
                          {96, 56},  // mine
                          {44, 79},
                          {44, 43},
                          {28, 10},  // mine
                          {60, 30},  // mine
                          {88, 72},  // mine
                          {75, 68},  // mine
                          {43, 65},
                          {48, 0},   // mine
                          {14, 15},  // mine
                          {49, 83},
                          {51, 35},
                          {95, 50},  // mine
                          {82, 20}}; // mine
  bool isMine[20] = {1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1};
  double targetCoords[2] = {45, 59};
  double expectedCoords[2] = {42, 63};


  vector<Point<2>> points;
  MineActionFAIL<2> action;
  for (int i = 0; i < 20; ++i)
      points.push_back(Point<2>(coords[i], isMine[i], &action));
  Point<2> target(targetCoords);
  Point<2> expected(expectedCoords);

  KDTree<2> tree(points);

  stringstream s;
  tree.printTree(s, colored_out::enable_t::DISABLE);
  INFO(s.str());

  action.trigger = true;

  REQUIRE(tree.findNearestNeighbor(target) == expected);
}


TEST_CASE("KDTree::findNearestNeighbor (3D), testing tie-breaking", "[weight=1][part=1]") {
  double coords[14][3] = {{0, 0, 100},   // mine
                          {0, 100, 100}, // mine
                          {0, 50, 50},   // mine
                          {0, 0, 50},    // mine
                          {100, 0, 100}, // mine
                          {100, 100, 0},
                          {0, 0, 0},
                          {0, 50, 0},    // mine
                          {50, 0, 0},    // mine
                          {50, 0, 50},   // mine
                          {100, 0, 0},   // mine
                          {50, 50, 0},
                          {0, 100, 0},   // mine
                          {50, 50, 50}};
  bool isMine[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  double targetCoords[3] = {50, 100, 0};
  double expectedCoords[3] = {0, 100, 0};
  double targetCoords2[3] = {14, 17, 20};
  double expectedCoords2[3] = {0, 0, 0};

  vector<Point<3>> points;
  MineActionFAIL<3> action;
  for (int i = 0; i < 14; ++i)
      points.push_back(Point<3>(coords[i], isMine[i], &action));
  Point<3> target(targetCoords);
  Point<3> expected(expectedCoords);
  Point<3> target2(targetCoords2);
  Point<3> expected2(expectedCoords2);

  KDTree<3> tree(points);

  stringstream s;
  tree.printTree(s, colored_out::enable_t::DISABLE);
  INFO(s.str());

  action.trigger = true;

  REQUIRE( tree.findNearestNeighbor(target) == expected );
  REQUIRE( tree.findNearestNeighbor(target2) == expected2 );
}


TEST_CASE("KDTree::findNearestNeighbor (3D), testing that left recursion does not include the root", "[weight=1][part=1]") {
  double coords[10][3] = {
    {5, 3, 1}, {3, 1, 10}, {2, 0, 8}, {4, 3, 0}, {0, 2, 9},
    {10, 10, 10}, {11, 11, 11}, {12, 12, 12}, {13, 13, 13}, {14, 14, 14}
  };
  double targetCoords[3] = {1, 1, 9};
  double expectedCoords[3] = {0, 2, 9};

  vector<Point<3>> points;
  for (int i = 0; i < 10; ++i)
      points.push_back(Point<3>(coords[i]));
  Point<3> target(targetCoords);
  Point<3> expected(expectedCoords);

  KDTree<3> tree(points);
  REQUIRE( tree.findNearestNeighbor(target) == expected );
}

TEST_CASE("KDTree::testTieBreakingConstructor", "[weight=10][part=1]") {
  // 在第 0 维都是 1，强制算法使用 operator< 排序
  vector<Point<2>> points;
  points.push_back(Point<2>(1.0, 10.0)); // d=0 相等，但这个点在 operator< 中较大
  points.push_back(Point<2>(1.0, 2.0));  // d=0 相等，这个点较小
  points.push_back(Point<2>(1.0, 5.0));  // 中位数应该是这个

  KDTree<2> tree(points);
  
  // 根据 floor((3-1)/2) = 1，根节点应该是 (1, 5)
  REQUIRE(tree.findNearestNeighbor(Point<2>(1.0, 5.0)) == Point<2>(1.0, 5.0));
}

TEST_CASE("KDTree::testNNBack-traversal", "[weight=10][part=1]") {
  vector<Point<2>> points;
  points.push_back(Point<2>(5.0, 5.0));   // Root (dim 0 split at x=5)
  points.push_back(Point<2>(2.0, 2.0));   // Left child
  points.push_back(Point<2>(8.0, 8.0));   // Right child
  points.push_back(Point<2>(4.5, 7.0)); // 在左子树，但物理上离右边某个点可能更近

  KDTree<2> tree(points);

  // 目标点
  Point<2> query(5.1, 7.0); 
  
  // 1. 初始下行会走到右子树 (5.1 > 5)
  // 2. 回溯时发现 (5, 5) 距离。
  // 3. 关键：query 到 x=5 平面的距离只有 0.1，小于当前半径。
  // 4. 必须进入左子树找到 (4.5, 7)，这才是真正的 NN。
  Point<2> expected(4.5, 7.0);
  REQUIRE(tree.findNearestNeighbor(query) == expected);
}

TEST_CASE("KDTree::testHighDimNN", "[weight=10][part=1]") {
  vector<Point<3>> points;
  points.push_back(Point<3>(0, 0, 0));
  points.push_back(Point<3>(10, 10, 10));
  points.push_back(Point<3>(5, 5, 5));
  points.push_back(Point<3>(1, 1, 8)); // 仅在 Z 轴很远

  KDTree<3> tree(points);

  Point<3> query(1, 1, 7);
  // (1,1,8) 的距离平方是 1, (0,0,0) 的距离平方是 1+1+49 = 51
  REQUIRE(tree.findNearestNeighbor(query) == Point<3>(1, 1, 8));
}

TEST_CASE("KDTree::testNNDistanceTie", "[weight=10][part=1]") {
  vector<Point<2>> points;
  points.push_back(Point<2>(0, 0));
  points.push_back(Point<2>(2, 2));

  KDTree<2> tree(points);

  // 目标点在正中间 (1, 1)
  Point<2> query(1, 1);
  
  // 到 (0,0) 距离平方是 2，到 (2,2) 也是 2
  // 根据 operator<，(0,0) < (2,2), 所以 shouldReplace 应该保持较小的那个点
  REQUIRE(tree.findNearestNeighbor(query) == Point<2>(0, 0));
}

