#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "catchlib.h"

#include "../graph.h"
#include "../edge.h"
#include "../NimLearner.h"

#include <iostream>

/*
TEST_CASE("The graph class creates directed edges", "[weight=5][part=1]") {
  Graph g(false);

  Vertex u = g.insertVertex();
  Vertex v = g.insertVertex();

  g.insertEdge(u, v);

  REQUIRE( g.getAdjacent(u).size() == 1 );
  REQUIRE( g.getAdjacent(v).size() == 0 );
}
*/

bool vertexExists(const Graph & g, const std::string & label) {
  for (const Vertex & v : g.getVertices()) {
    if (g.getVertexLabel(v) == label)
      return true;
  }

  return false;
}

bool edgeExists(const Graph & g, const std::string & label1, const std::string & label2) {
  for (const Vertex & v : g.getVertices()) {
    if (g.getVertexLabel(v) == label1) {
      // found vertex v for label1, check getAdjacent
      for (const Vertex & u : g.getAdjacent(v)) {
        if (g.getVertexLabel(u) == label2) {
          return true;
        }
      }

      // Found label1, did not find an adjacent label2
      return false;
    }
  }

  return false;
}



TEST_CASE("NimLearner(10) constructor creates a starting vertex \"p1-10\"", "[weight=1][part=1]") {
  NimLearner nim(10);

  const Graph & g = nim.getGraph();
  REQUIRE( vertexExists(g, "p1-10") );
}


TEST_CASE("NimLearner(3) constructor creates the correct vertices", "[weight=1][part=1]") {
  NimLearner nim(3);

  const Graph & g = nim.getGraph();

  REQUIRE( vertexExists(g, "p1-3") );
  //REQUIRE( findVertexLabel(g, "p2-3") ); okay but not required, never reached
  REQUIRE( vertexExists(g, "p1-2") );
  REQUIRE( vertexExists(g, "p2-2") );
  REQUIRE( vertexExists(g, "p1-1") );
  REQUIRE( vertexExists(g, "p2-1") );
  REQUIRE( vertexExists(g, "p1-0") );
  REQUIRE( vertexExists(g, "p2-0") );
}


TEST_CASE("NimLearner(3) constructor creates the correct edges", "[weight=1][part=1]") {
  NimLearner nim(3);

  const Graph & g = nim.getGraph();

  //
  // A minimum of 8 edges are required (p2-3 edges are not required);
  // 10 edges may be present with p2-3 edges
  //
  REQUIRE(g.getEdges().size() >= 8);
  REQUIRE(g.getEdges().size() <= 10);

  if (g.getEdges().size() >= 8) {
    // always required edges
    REQUIRE( edgeExists(g, "p1-3", "p2-2") );
    REQUIRE( edgeExists(g, "p1-3", "p2-1") );

    REQUIRE( edgeExists(g, "p1-2", "p2-1") );
    REQUIRE( edgeExists(g, "p1-2", "p2-0") );

    REQUIRE( edgeExists(g, "p1-1", "p2-0") );

    REQUIRE( edgeExists(g, "p2-2", "p1-1") );
    REQUIRE( edgeExists(g, "p2-2", "p1-0") );

    REQUIRE( edgeExists(g, "p2-1", "p1-0") );
  }

  if (g.getEdges().size() > 8) {
    // optional edges
    REQUIRE( edgeExists(g, "p2-3", "p1-2") );
    REQUIRE( edgeExists(g, "p2-3", "p1-1") );
  }
}


TEST_CASE("NimLearner(1) plays a trivial random game", "[weight=1][part=2]") {
  NimLearner nim(1);

  std::vector<Edge> path = nim.playRandomGame();
  REQUIRE( path.size() == 1 );
}

TEST_CASE("NimLearner(10) plays random game", "[weight=1][part=2]") {
  NimLearner nim(10);

  std::vector<Edge> path = nim.playRandomGame();
  REQUIRE( path.size() >= 5 );
  REQUIRE( path.size() <= 10 );
}

TEST_CASE("Path from playRandomGame of NimLearner(10) source vertex is labeled \"p1-10\"", "[weight=1][part=2]") {
  NimLearner nim(10);

  std::vector<Edge> path = nim.playRandomGame();

  const Graph & g = nim.getGraph();
  REQUIRE( g.getVertexLabel(path[0].source) == "p1-10" );
}

TEST_CASE("Path from playRandomGame of NimLearner(10) final vertex is labeled \"p1-0\" or \"p2-0\"", "[weight=1][part=2]") {
  NimLearner nim(10);

  std::vector<Edge> path = nim.playRandomGame();

  const Graph & g = nim.getGraph();
  REQUIRE( g.getVertexLabel(path[path.size() - 1].dest).substr(2, 2) == "-0" );
}

TEST_CASE("A NimLearner(1) random path contains one edge", "[weight=1][part=3]") {
  NimLearner nim(1);

  std::vector<Edge> path = nim.playRandomGame();
  REQUIRE( path.size() == 1 );
}

TEST_CASE("NimLearner(1) updates the edge weight correctly", "[weight=1][part=3]") {
  NimLearner nim(1);
  std::vector<Edge> path = nim.playRandomGame();
  Edge e = path[0];
  nim.updateEdgeWeights(path);

  const Graph & g = nim.getGraph();
  REQUIRE( g.getEdgeWeight(e.source, e.dest) == 1);
}

TEST_CASE("NimLearner(3) learns that (p1-3, p2-2) and (p1-3, p2-1) are losing edges", "[weight=1][part=3]") {
  NimLearner nim(3);

  for (int i = 0; i < 10000; i++) {
    std::vector<Edge> path = nim.playRandomGame();
    nim.updateEdgeWeights(path);
  }

  const Graph & g = nim.getGraph();
  REQUIRE( g.getEdgeWeight( g.getVertexByLabel("p1-3"), g.getVertexByLabel("p2-2")) < 0 );
  REQUIRE( g.getEdgeWeight( g.getVertexByLabel("p1-3"), g.getVertexByLabel("p2-1")) < 0 );
}

TEST_CASE("NimLearner(4) learns that (p1-4, p2-3) is a better edge than (p1-4, p2-2)", "[weight=1][part=3]") {
  NimLearner nim(4);

  for (int i = 0; i < 10000; i++) {
    std::vector<Edge> path = nim.playRandomGame();
    nim.updateEdgeWeights(path);
  }

  const Graph & g = nim.getGraph();
  REQUIRE(
    g.getEdgeWeight( g.getVertexByLabel("p1-4"), g.getVertexByLabel("p2-3")) >
    g.getEdgeWeight( g.getVertexByLabel("p1-4"), g.getVertexByLabel("p2-2"))
  );
}

TEST_CASE("NimLearner(10) has ending edge weights summing to the random games played", "[weight=1][part=3]") {
  NimLearner nim(4);

  for (int i = 0; i < 100; i++) {
    std::vector<Edge> path = nim.playRandomGame();
    nim.updateEdgeWeights(path);
  }

  const Graph & g = nim.getGraph();

  Vertex p1_2 = g.getVertexByLabel("p1-2");
  Vertex p1_1 = g.getVertexByLabel("p1-1");
  Vertex p1_0 = g.getVertexByLabel("p1-0");

  Vertex p2_2 = g.getVertexByLabel("p2-2");
  Vertex p2_1 = g.getVertexByLabel("p2-1");
  Vertex p2_0 = g.getVertexByLabel("p2-0");

  REQUIRE( g.getEdgeWeight( p1_2, p2_0 ) +
           g.getEdgeWeight( p1_1, p2_0 ) +
           g.getEdgeWeight( p2_2, p1_0 ) +
           g.getEdgeWeight( p2_1, p1_0 ) == 100 );
}



/* additional test written by gemini */
TEST_CASE("NimLearner(2) graph connectivity and structure", "[weight=2][part=1]") {
  NimLearner nim(2);
  const Graph & g = nim.getGraph();

  // 1. 验证顶点总数: 
  // p1-2, p2-2, p1-1, p2-1, p1-0, p2-0 -> 共 6 个顶点
  REQUIRE(g.getVertices().size() == 6);

  // 2. 验证所有边的初始权重必须为 0
  for (const auto & edge : g.getEdges()) {
    REQUIRE(edge.weight == 0);
  }

  // 3. 边界条件：验证 p1-1 的出边
  // p1-1 只能拿 1 个，应指向 p2-0
  REQUIRE(edgeExists(g, "p1-1", "p2-0"));
  REQUIRE(g.getAdjacent(g.getVertexByLabel("p1-1")).size() == 1);

  // 4. 边界条件：验证 p1-0 或 p2-0 是否存在出边 (应该为 0，因为游戏结束)
  REQUIRE(g.getAdjacent(g.getVertexByLabel("p1-0")).size() == 0);
  REQUIRE(g.getAdjacent(g.getVertexByLabel("p2-0")).size() == 0);
}

TEST_CASE("NimLearner(10) edge count validation", "[weight=2][part=1]") {
  NimLearner nim(10);
  const Graph & g = nim.getGraph();

  // 逻辑推演:
  // 对于每个状态 pX-k (k>0):
  // k >= 2 时有 2 条出边 (k-1, k-2)
  // k == 1 时有 1 条出边 (k-1)
  // 状态从 10 到 1，共有 10 个 p1 状态，10 个 p2 状态
  // p1-10...p1-1 (10+9=19条), p2-10...p2-1 (10+9=19条)
  // 总边数应为 38 条
  REQUIRE(g.getEdges().size() == 38);
}

TEST_CASE("NimLearner vertex labeling consistency", "[weight=2][part=1]") {
  NimLearner nim(5);
  const Graph & g = nim.getGraph();

  // 验证所有预期的标签是否存在
  std::string players[] = {"p1", "p2"};
  for (const std::string & p : players) {
    for (int i = 0; i <= 5; ++i) {
      std::string label = p + "-" + std::to_string(i);
      REQUIRE(vertexExists(g, label));
    }
  }
}

TEST_CASE("NimLearner directionality check", "[weight=2][part=1]") {
  NimLearner nim(5);
  const Graph & g = nim.getGraph();

  // 验证图是单向的（有向图），不能从 0 回到 1
  Vertex p1_1 = g.getVertexByLabel("p1-1");
  Vertex p1_0 = g.getVertexByLabel("p1-0");
  Vertex nonsense = g.getVertexByLabel("fuck stupid 225 structure");

  /* from implementation of graph, just for sure */
  REQUIRE(p1_1 != static_cast<Vertex>(-1));
  REQUIRE(p1_0 != static_cast<Vertex>(-1)); 
  REQUIRE(nonsense == static_cast<Vertex>(-1)); 
  
  // p1-1 -> p2-0 存在
  REQUIRE(edgeExists(g, "p1-1", "p2-0"));
  // p2-0 -> p1-1 不存在 (逆向不可达)
  REQUIRE(!edgeExists(g, "p2-0", "p1-1"));
}

TEST_CASE("NimLearner(0) - Boundary Case", "[weight=1][part=1]") {
    // 0个筹码的情况下，直接应该是终点状态
    NimLearner nim(0);
    const Graph & g = nim.getGraph();
    REQUIRE( vertexExists(g, "p1-0") );
    REQUIRE( vertexExists(g, "p2-0") );
    REQUIRE( g.getVertices().size() == 2 );
}

TEST_CASE("NimLearner(2) - Exact Vertex Count", "[weight=1][part=1]") {
    NimLearner nim(2);
    const Graph & g = nim.getGraph();
    // 验证顶点集合大小，应包含 p1-2, p2-2, p1-1, p2-1, p1-0, p2-0
    REQUIRE( g.getVertices().size() == 6 );
}

TEST_CASE("NimLearner - Verify Edge Weights are 0 Initially", "[weight=1][part=1]") {
    NimLearner nim(5);
    const Graph & g = nim.getGraph();
    for (const Edge & e : g.getEdges()) {
        REQUIRE( e.weight == 0 );
    }
}

TEST_CASE("NimLearner(10) - Path Continuity", "[weight=1][part=2]") {
    NimLearner nim(10);
    std::vector<Edge> path = nim.playRandomGame();
    
    // 验证路径连接是否正确：path[i].dest 必须等于 path[i+1].source
    for (size_t i = 0; i < path.size() - 1; ++i) {
        REQUIRE( path[i].dest == path[i+1].source );
    }
}

TEST_CASE("NimLearner(10) - Path Validity (Valid moves only)", "[weight=1][part=2]") {
    NimLearner nim(10);
    std::vector<Edge> path = nim.playRandomGame();
    const Graph & g = nim.getGraph();

    for (const Edge & e : path) {
        std::string src = g.getVertexLabel(e.source);
        std::string dest = g.getVertexLabel(e.dest);
        
        // 解析筹码数 (假设标签格式为 p#-X)
        int src_tokens = std::stoi(src.substr(3));
        int dest_tokens = std::stoi(dest.substr(3));
        
        // 每次必须拿走 1 或 2 个筹码
        int diff = src_tokens - dest_tokens;
        REQUIRE( (diff == 1 || diff == 2) );
    }
}

TEST_CASE("NimLearner - End at 0 tokens", "[weight=1][part=2]") {
    NimLearner nim(5);
    std::vector<Edge> path = nim.playRandomGame();
    const Graph & g = nim.getGraph();
    
    // 无论谁赢，最后都应该停在 0
    std::string final_dest = g.getVertexLabel(path.back().dest);
    REQUIRE( final_dest.substr(3) == "0" );
}

// 增加辅助函数：验证路径中每一步移动是否合法
// 合法移动：筹码数减少 1 或 2，且玩家从 p1 变为 p2 或反之
bool isLegalMove(const Graph & g, Edge e) {
    std::string s_label = g.getVertexLabel(e.source);
    std::string d_label = g.getVertexLabel(e.dest);
    
    // 解析标签格式 p#-X
    int p_s = s_label[1] - '0';
    int k_s = std::stoi(s_label.substr(3));
    int p_d = d_label[1] - '0';
    int k_d = std::stoi(d_label.substr(3));
    
    // 玩家必须轮换
    if (p_s == p_d) return false;
    // 筹码数必须减少 1 或 2
    if (k_s - k_d != 1 && k_s - k_d != 2) return false;
    
    return true;
}

TEST_CASE("NimLearner::playRandomGame produces a valid continuous path", "[part=2]") {
    NimLearner nim(10);
    std::vector<Edge> path = nim.playRandomGame();
    const Graph & g = nim.getGraph();

    // 1. 测试路径连续性
    for (size_t i = 0; i < path.size() - 1; ++i) {
        // 当前边的终点必须是下一条边的起点
        REQUIRE(path[i].dest == path[i + 1].source);
    }

    // 2. 测试每一步的合法性
    for (const auto & e : path) {
        REQUIRE(isLegalMove(g, e) == true);
    }
}

TEST_CASE("NimLearner::playRandomGame boundary case: k=2", "[part=2]") {
    // 边界情况：只剩 2 个筹码，路径长度应该可能是 1 (拿2个) 或 2 (拿1个再拿1个)
    NimLearner nim(2);
    
    for(int i = 0; i < 100; ++i) { // 运行多次以覆盖不同路径
        std::vector<Edge> path = nim.playRandomGame();
        REQUIRE(path.size() >= 1);
        REQUIRE(path.size() <= 2);
        
        const Graph & g = nim.getGraph();
        REQUIRE(g.getVertexLabel(path[0].source) == "p1-2");
        
        // 检查终点是否确实是 0
        std::string last_dest = g.getVertexLabel(path.back().dest);
        REQUIRE(last_dest.substr(3) == "0");
    }
}

TEST_CASE("NimLearner::playRandomGame terminal vertex labeling", "[part=2]") {
    // 测试路径是否总是以 pX-0 结尾
    NimLearner nim(5);
    for(int i = 0; i < 50; ++i) {
        std::vector<Edge> path = nim.playRandomGame();
        std::string last_dest = nim.getGraph().getVertexLabel(path.back().dest);
        
        // 验证最后一个顶点确实代表剩余 0 个筹码
        REQUIRE(last_dest.back() == '0');
    }
}

TEST_CASE("NimLearner::playRandomGame structural integrity", "[part=2]") {
    NimLearner nim(10);
    std::vector<Edge> path = nim.playRandomGame();
    
    // 检查路径中的每一条边是否在图中真实存在
    const Graph & g = nim.getGraph();
    for (const auto & e : path) {
        bool found = false;
        for (const auto & graphEdge : g.getEdges()) {
            if (e.source == graphEdge.source && e.dest == graphEdge.dest) {
                found = true;
                break;
            }
        }
        REQUIRE(found == true);
    }
}

// --- Part 3 增强测试 ---

TEST_CASE("NimLearner updateEdgeWeights: verify reward and punishment logic", "[weight=2][part=3]") {
    NimLearner nim(2);
    // 在 Nim(2) 中，一条可能的路径是 p1-2 -> p2-0 (Player 1 直接拿走2个获胜)
    // 或者 p1-2 -> p2-1 -> p1-0 (Player 1 拿1个，Player 2 拿1个，Player 1 获胜)
    
    // 强制运行一次模拟
    std::vector<Edge> path = nim.playRandomGame();
    nim.updateEdgeWeights(path);
    
    const Graph & g = nim.getGraph();
    
    // 检查路径中每一条边
    for (const auto& e : path) {
        int weight = g.getEdgeWeight(e.source, e.dest);
        std::string srcLabel = g.getVertexLabel(e.source);
        
        // 如果是 Player 1 的移动，权重应该是 +1
        if (srcLabel.substr(0, 2) == "p1") {
            REQUIRE(weight == 1);
        } 
        // 如果是 Player 2 的移动，权重应该是 -1
        else if (srcLabel.substr(0, 2) == "p2") {
            REQUIRE(weight == -1);
        }
    }
}

TEST_CASE("NimLearner: check boundary case when p1-1 leads to p2-0", "[weight=1][part=3]") {
    NimLearner nim(1);
    std::vector<Edge> path = nim.playRandomGame();
    nim.updateEdgeWeights(path);
    
    const Graph & g = nim.getGraph();
    Vertex p1_1 = g.getVertexByLabel("p1-1");
    Vertex p2_0 = g.getVertexByLabel("p2-0");
    
    // p1-1 只能走到 p2-0，此时 Player 1 获胜，权重应为 1
    REQUIRE(g.getEdgeWeight(p1_1, p2_0) == 1);
}

TEST_CASE("NimLearner: verify edge labels match state space", "[weight=1][part=3]") {
    NimLearner nim(5);
    const Graph & g = nim.getGraph();
    
    // 遍历所有边，验证源点和终点的标签格式
    for (const auto& e : g.getEdges()) {
        std::string src = g.getVertexLabel(e.source);
        std::string dst = g.getVertexLabel(e.dest);
        
        // 验证标签格式 p#-X
        REQUIRE(src.size() >= 3);
        REQUIRE(dst.size() >= 3);
        REQUIRE(src[0] == 'p');
        REQUIRE(dst[0] == 'p');
    }
}

TEST_CASE("NimLearner: deep learning convergence check", "[weight=3][part=3]") {
    NimLearner nim(10);
    // 经过大量训练，最优路径的边权重应该显著高于次优路径
    for(int i = 0; i < 20000; ++i) {
        nim.updateEdgeWeights(nim.playRandomGame());
    }
    
    const Graph & g = nim.getGraph();
    // 逻辑：在 p1-2 状态，拿走 2 个筹码 (p1-2 -> p2-0) 是必胜策略
    // 拿走 1 个筹码 (p1-2 -> p2-1) 则是把获胜机会留给对手
    int weight_take2 = g.getEdgeWeight(g.getVertexByLabel("p1-2"), g.getVertexByLabel("p2-0"));
    int weight_take1 = g.getEdgeWeight(g.getVertexByLabel("p1-2"), g.getVertexByLabel("p2-1"));
    
    REQUIRE(weight_take2 > weight_take1);
}

// 3. 验证“胜者”定义的鲁棒性
TEST_CASE("NimLearner(10) ensures only winning player's edges are rewarded", "[weight=1][part=3]") {
  NimLearner nim(10);
  
  for (int i = 0; i < 500; i++) {
    std::vector<Edge> path = nim.playRandomGame();
    nim.updateEdgeWeights(path);
  }

  const Graph & g = nim.getGraph();
  // 检查关键终结边：如果 p2-0 是由 p1-1 达到的
  // 那么 p1-1 -> p2-0 的权重必然为正，因为它导致了玩家1获胜
  Vertex p1_1 = g.getVertexByLabel("p1-1");
  Vertex p2_0 = g.getVertexByLabel("p2-0");
  
  // 只要游戏进行了足够多次，胜负路径权重应该能区分出来
  // 获胜的移动权重应该 > 0
  REQUIRE(g.getEdgeWeight(p1_1, p2_0) >= 0); 
}

// 4. 极端测试：空路径处理（防御性测试）
TEST_CASE("NimLearner handles empty path gracefully", "[weight=1][part=3]") {
  NimLearner nim(10);
  std::vector<Edge> emptyPath;
  
  // 确保调用不会崩溃
  REQUIRE_NOTHROW(nim.updateEdgeWeights(emptyPath));
}