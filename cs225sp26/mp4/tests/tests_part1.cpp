#include "catch.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"

#include "../imageTraversal/BFS.h"
#include "../imageTraversal/DFS.h"

using namespace cs225;


/**
 * Returns a 4x4 image with a white center and black border:
 * 
 *     X X X X
 *     X - - X       X: black
 *     X - - X       -: white
 *     X X X X
 * 
 * The black/X (h=180, s=1, l=0) is as different from
 * white/- (h=0, s=0, l=1) as possible.
**/
PNG getTestPNG() {
  PNG png(4, 4);
  HSLAPixel blackPixel(180, 1, 0);
  
  for (unsigned i = 0; i < 4; i++) {    
    *(png.getPixel(i, 0)) = blackPixel;
    *(png.getPixel(0, i)) = blackPixel;
    *(png.getPixel(i, 3)) = blackPixel;
    *(png.getPixel(3, i)) = blackPixel;
  }
    
  return png;
}


TEST_CASE("DFS iterator starts at the start point", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(1, 1);
  
  DFS dfs(png, startPoint, 0.2);
  REQUIRE( *(dfs.begin()) == startPoint );
}

TEST_CASE("DFS visits the right pixel first", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(1, 1);
  
  DFS dfs(png, startPoint, 0.2);
  ImageTraversal::Iterator it = dfs.begin();
  ++it;
  REQUIRE( *it == Point(1, 2) );
}


TEST_CASE("BFS iterator starts at the start point", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(1, 1);
  
  BFS bfs(png, startPoint, 0.2);
  REQUIRE( *(bfs.begin()) == startPoint );
}

TEST_CASE("BFS visits the right pixel first", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(1, 1);
  
  BFS bfs(png, startPoint, 0.2);
  ImageTraversal::Iterator it = bfs.begin();
  ++it;
  REQUIRE( *it == Point(2, 1) );
}


TEST_CASE("DFS visits all points within a tolerance", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(1, 1);
  
  DFS t(png, startPoint, 0.2);
  unsigned count = 0;
  for (const Point & p : t) {
    count++;
  }
  REQUIRE( count == 4 );
}

TEST_CASE("BFS visits all points within a tolerance", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(1, 1);
  
  BFS t(png, startPoint, 0.2);
  unsigned count = 0;
  for (const Point & p : t) {
    count++;
  }
  REQUIRE( count == 4 );
}


TEST_CASE("DFS visits all points within a tolerance (includes pixels on image edge)", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(0, 0);
  
  DFS t(png, startPoint, 0.2);
  unsigned count = 0;
  for (const Point & p : t) { count++; }

  REQUIRE( count == 12 );
}

TEST_CASE("BFS visits all points within a tolerance (includes pixels on image edge)", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(0, 0);
  
  BFS t(png, startPoint, 0.2);
  unsigned count = 0;
  for (const Point & p : t) { count++; }

  REQUIRE( count == 12 );
}


TEST_CASE("DFS iterator visits all points in the correct order", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(0, 0);
  
  DFS t(png, startPoint, 0.2);
  ImageTraversal::Iterator it = t.begin();

  REQUIRE( *it == Point(0, 0) ); ++it;
  REQUIRE( *it == Point(0, 1) ); ++it;
  REQUIRE( *it == Point(0, 2) ); ++it;
  REQUIRE( *it == Point(0, 3) ); ++it;
  REQUIRE( *it == Point(1, 3) ); ++it;
  REQUIRE( *it == Point(2, 3) ); ++it;  
  REQUIRE( *it == Point(3, 3) ); ++it;
  REQUIRE( *it == Point(3, 2) ); ++it;
  REQUIRE( *it == Point(3, 1) ); ++it;  
  REQUIRE( *it == Point(3, 0) ); ++it;
  REQUIRE( *it == Point(2, 0) ); ++it;
  REQUIRE( *it == Point(1, 0) ); ++it;  
}

TEST_CASE("BFS iterator visits all points in the correct order", "[weight=1][part=1]") {
  PNG png = getTestPNG();
  Point startPoint(0, 0);
  
  BFS t(png, startPoint, 0.2);
  ImageTraversal::Iterator it = t.begin();

  REQUIRE( *it == Point(0, 0) ); ++it;

  REQUIRE( *it == Point(1, 0) ); ++it;  
  REQUIRE( *it == Point(0, 1) ); ++it;

  REQUIRE( *it == Point(2, 0) ); ++it;  
  REQUIRE( *it == Point(0, 2) ); ++it;

  REQUIRE( *it == Point(3, 0) ); ++it;
  REQUIRE( *it == Point(0, 3) ); ++it;

  REQUIRE( *it == Point(3, 1) ); ++it;
  REQUIRE( *it == Point(1, 3) ); ++it;
  
  REQUIRE( *it == Point(3, 2) ); ++it;
  REQUIRE( *it == Point(2, 3) ); ++it;

  REQUIRE( *it == Point(3, 3) ); ++it;
}

TEST_CASE("DFS iterator visits all points in correct order (4x4 Doc Example)", "[part=1]") {
  /* * 创建一个 4x4 纯白 PNG (0.0 亮度)，
   * 但在 (0, 2) 放置一个黑色像素 (1.0 亮度)，其 delta 将超过默认 tolerance。
   */
  PNG png(4, 4);
  HSLAPixel blackPixel(0, 0, 0); // 黑色
  *png.getPixel(0, 2) = blackPixel; 

  Point startPoint(0, 3);
  double tolerance = 0.2;
  DFS dfs(png, startPoint, tolerance);

  auto it = dfs.begin();
  // 1. 起点
  REQUIRE(*it == Point(0, 3));
  
  // 2. 访问 (0, 3) 后，邻居顺序：右(1, 3), 下(out), 左(out), 上(0, 2)
  // 因为 (0, 2) 是黑色，delta 超过 tolerance，所以只剩下 (1, 3) 入栈
  ++it;
  REQUIRE(*it == Point(1, 3));

  // 3. 访问 (1, 3) 后，邻居顺序：右(2, 3), 下(out), 左(0, 3-visited), 上(1, 2)
  // 栈顶现在是 (1, 2)
  ++it;
  REQUIRE(*it == Point(1, 2));
}

TEST_CASE("BFS iterator visits all points in correct order (4x4 Doc Example)", "[part=1]") {
  PNG png(4, 4);
  Point startPoint(0, 3);
  double tolerance = 0.2;
  BFS bfs(png, startPoint, tolerance);

  auto it = bfs.begin();
  // 1. 起点
  REQUIRE(*it == Point(0, 3));

  // 2. 访问 (0, 3) 后，邻居：右(1, 3), 上(0, 2) 进入队列
  ++it;
  REQUIRE(*it == Point(1, 3));

  // 3. 队列先入先出，下一个应该是 (0, 2)
  ++it;
  REQUIRE(*it == Point(0, 2));

  // 4. (1, 3) 的邻居：右(2, 3), 上(1, 2) 进入队列
  // 此时队列：(2, 3), (1, 2)
  ++it;
  REQUIRE(*it == Point(2, 3));
}

TEST_CASE("Iterators are independent", "[part=1]") {
  PNG png(10, 10);
  Point start(5, 5);
  DFS dfs(png, start, 0.2);

  auto it1 = dfs.begin();
  auto it2 = dfs.begin();

  REQUIRE(*it1 == start);
  REQUIRE(*it2 == start);

  // 移动 it1
  ++it1;
  Point p1 = *it1;

  // it2 应该还停留在原点，不受 it1 影响
  REQUIRE(*it2 == start);
  REQUIRE(!(*it1 == *it2));

  // 再次移动 it2
  ++it2;
  REQUIRE(*it2 == p1);
}

TEST_CASE("Traversal stays within tolerance", "[part=1]") {
  PNG png(3, 1);
  // (0,0) 白, (1,0) 灰, (2,0) 黑
  *png.getPixel(0, 0) = HSLAPixel(0, 0, 1.0); 
  *png.getPixel(1, 0) = HSLAPixel(0, 0, 0.5); // Delta = 0.5
  *png.getPixel(2, 0) = HSLAPixel(0, 0, 0.0); // Delta = 1.0

  Point start(0, 0);
  
  SECTION("Low tolerance") {
    DFS dfs(png, start, 0.1);
    auto it = dfs.begin();
    REQUIRE(*it == Point(0, 0));
    ++it;
    REQUIRE(it == dfs.end()); // 只能访问起点
  }

  SECTION("High tolerance") {
    BFS bfs(png, start, 0.6);
    auto it = bfs.begin();
    REQUIRE(*it == Point(0, 0));
    ++it;
    REQUIRE(*it == Point(1, 0));
    ++it;
    REQUIRE(it == bfs.end()); // (2,0) 超出 0.6 的容差
  }
}
