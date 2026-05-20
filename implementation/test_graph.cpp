#include "graph.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

int main() {
    std::cout << "========= 开始硬核图库功能测试 =========" << std::endl;

    // ----------------------------------------------------------------
    // 测试 1：无向图基础插入、删除与 ID 复用池
    // ----------------------------------------------------------------
    {
        Graph<std::string, double, false> g;
        
        VertexID u = g.InsertVertex("Node_A");
        VertexID v = g.InsertVertex("Node_B");
        VertexID w = g.InsertVertex("Node_C");

        assert(u == 0 && v == 1 && w == 2);
        assert(g.num_vertices() == 3);

        EdgeID e1 = g.InsertEdge(u, v, 1.5);
        EdgeID e2 = g.InsertEdge(v, w, 2.5);
        
        assert(g.num_edges() == 2);
        assert(g.adjacent(u, v) == true);
        assert(g.adjacent(u, w) == false); // 不直接相邻

        // 测试删除顶点及其连锁反应
        g.RemoveVertex(v); // 删掉节点 1，e1 和 e2 应该顺带死掉
        assert(g.num_vertices() == 2);
        assert(g.num_edges() == 0);
        assert(g.adjacent(u, w) == false);

        // 测试 ID 复用池
        VertexID reuse_v = g.InsertVertex("Node_B_New");
        assert(reuse_v == 1); // 必须完美复用之前死掉的 ID 1！
        assert(g.num_vertices() == 3);
        
        std::cout << "👉 测试 1 (基础增删与 ID 复用) 完美通过！" << std::endl;
    }

    // ----------------------------------------------------------------
    // 测试 2：多重边 (Parallel Edges) 与自环 (Self-loops) 边界测试
    // ----------------------------------------------------------------
    {
        Graph<std::string, int, false> g;
        VertexID u = g.InsertVertex("LoopNode");

        // 插入一条无向自环边
        EdgeID e_loop = g.InsertEdge(u, u, 42);
        assert(g.num_edges() == 1);
        assert(g.adjacent(u, u) == true);

        // 插入平行边（多重图特性）
        VertexID v = g.InsertVertex("TargetNode");
        EdgeID e1 = g.InsertEdge(u, v, 100);
        EdgeID e2 = g.InsertEdge(u, v, 200);
        assert(g.num_edges() == 3);

        // 批量删除连结 u 和 v 的所有平行边
        g.RemoveEdge(u, v);
        assert(g.num_edges() == 1); // 应该只剩下自环边
        assert(g.adjacent(u, v) == false);

        // 删除自环边（检验你的迭代器擦除是否会引发 Segfault）
        g.RemoveEdge(e_loop);
        assert(g.num_edges() == 0);

        std::cout << "👉 测试 2 (多重边与自环高危测试) 完美通过！" << std::endl;
    }

    // ----------------------------------------------------------------
    // 测试 3：有向图方向校验补丁测试
    // ----------------------------------------------------------------
    {
        Graph<std::string, int, true> g; // 开启有向图模式
        VertexID u = g.InsertVertex("Src");
        VertexID v = g.InsertVertex("Dst");

        g.InsertEdge(u, v, 500); // u -> v

        assert(g.adjacent(u, v) == true);  // 正向通
        assert(g.adjacent(v, u) == false); // 反向不通！(检验你的补丁是否生效)

        auto incident = g.GetIncidentEdges(u);
        assert(incident.size() == 1); // u 有出边

        auto incident_v = g.GetIncidentEdges(v);
        assert(incident_v.size() == 0); // v 没有出边

        std::cout << "👉 测试 3 (有向图方向验证) 完美通过！" << std::endl;
    }

    std::cout << "\n🎉 🎉 恭喜你！所有边界测试全部通过！代码工业级安全！" << std::endl;


    std::cout << "========= 开启地狱级高强度图库边界测试 =========" << std::endl;

    // ----------------------------------------------------------------
    // 地狱测试 1：高频重复删除防御（防御性工程测试）
    // ----------------------------------------------------------------
    {
        Graph<char, int, false> g;
        VertexID u = g.InsertVertex('A');
        VertexID v = g.InsertVertex('B');
        EdgeID e = g.InsertEdge(u, v, 10);

        // 正常删除
        g.RemoveEdge(e);
        assert(g.num_edges() == 0);

        // 🚨 攻击点：对一个已经删除的 EdgeID 再次进行调用
        // 期望：应该由于 edge_exists(e) 返回 false 而安全退出，不应该崩溃
        g.RemoveEdge(e); 
        
        // 🚨 攻击点：通过顶点对删除一个已经不存在的边
        g.RemoveEdge(u, v);

        // 🚨 攻击点：重复删除同一个已被删除的顶点
        g.RemoveVertex(u);
        g.RemoveVertex(u); 
        
        std::cout << "🔥 地狱测试 1 (重复删除防御) 安全通过！" << std::endl;
    }

    // ----------------------------------------------------------------
    // 地狱测试 2：有向图自环的多重组合与精细删除
    // ----------------------------------------------------------------
    {
        Graph<char, int, true> g; // 有向图
        VertexID u = g.InsertVertex('A');

        // 插入多条有向自环边 (u -> u)
        EdgeID e1 = g.InsertEdge(u, u, 100);
        EdgeID e2 = g.InsertEdge(u, u, 200);
        assert(g.num_edges() == 2);

        // 🚨 攻击点：在有向图中通过顶点对删除自环边
        // 你的逻辑中：edge.u == u && edge.v == v，应该把 e1 和 e2 全都干净地干掉
        g.RemoveEdge(u, u);
        assert(g.num_edges() == 0);

        std::cout << "🔥 地狱测试 2 (有向自环精细删除) 安全通过！" << std::endl;
    }

    // ----------------------------------------------------------------
    // 地狱测试 3：深度 ID 复用后的迭代器完整性（内存污染测试）
    // ----------------------------------------------------------------
    {
        Graph<char, int, false> g;
        VertexID u = g.InsertVertex('A'); // ID: 0
        VertexID v = g.InsertVertex('B'); // ID: 1
        EdgeID e1 = g.InsertEdge(u, v, 10); // ID: 0

        // 删掉这条边，让 EdgeID 0 进入 free_list
        g.RemoveEdge(e1);

        // 🚨 攻击点：插入一条新边，必然复用 EdgeID 0
        // 这时我们要看新边的 pos_u_adj 和 pos_v_adj 是否被干净地覆写
        EdgeID e2 = g.InsertEdge(u, v, 99); 
        assert(e2 == 0); // 确认复用

        // 尝试正常删除这辆“套牌车”（复用 ID 的新边）
        g.RemoveEdge(e2); 
        assert(g.num_edges() == 0);

        std::cout << "🔥 地狱测试 3 (复用边迭代器覆写测试) 安全通过！" << std::endl;
    }

    // ----------------------------------------------------------------
    // 地狱测试 4：经典多重图在 `adjacent` 时的多重覆盖测试
    // ----------------------------------------------------------------
    {
        Graph<char, int, true> g; // 有向多重图
        VertexID u = g.InsertVertex('A');
        VertexID v = g.InsertVertex('B');

        // 建立两条相同方向的平行边
        g.InsertEdge(u, v, 1);
        EdgeID e_second = g.InsertEdge(u, v, 2);

        assert(g.adjacent(u, v) == true);

        // 🚨 攻击点：只删其中一条平行边
        g.RemoveEdge(e_second);

        // 期望：因为还有一条平行边撑着，它们依然应该是 adjacent 的！
        assert(g.adjacent(u, v) == true);

        std::cout << "🔥 地狱测试 4 (多重边部分删除后邻接测试) 安全通过！" << std::endl;
    }

    std::cout << "\n👑 奇迹！你的全手写图库完美通过了全部地狱级极限破坏测试！" << std::endl;
    
    return 0;
}