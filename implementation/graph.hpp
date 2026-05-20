#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <list>

using VertexID = std::size_t;
using EdgeID = std::size_t;

/* AdjInfo only carries adjacent info, without direction */
struct AdjInfo
{
	VertexID neighbor_id;
	EdgeID edge_id;
};

template<typename VertexData>
struct Vertex
{
	VertexID id;
	VertexData data;
	bool is_deleted = false;

	Vertex(VertexID id, const VertexData &data) : id(id), data(data) {}
};

template<typename EdgeData>
struct Edge
{
	EdgeID id;

	/* edge takes direction info */
	VertexID u; 	/* source */
	VertexID v; 	/* destination */

	EdgeData data; 	/* support operation +/</>/== and value 0 */
	bool is_deleted = false;
	std::list<AdjInfo>::iterator pos_u_adj = {};
	std::list<AdjInfo>::iterator pos_v_adj = {};

	Edge(EdgeID id, VertexID u, VertexID v, 
		const EdgeData &data) : id(id), u(u), v(v), data(data) {}
};

template<typename VertexData, typename EdgeData, bool IsDirected = false>
class Graph
{
	private:	
		/* key components */
		std::vector<Vertex<VertexData>> vertices_;
		std::vector<Edge<EdgeData>> edges_;
		std::vector<std::list<AdjInfo>> adj_; 
		std::vector<VertexID> free_vertex_ids_;
		std::vector<EdgeID>   free_edge_ids_;

	public:
		Graph() = default;
		Graph(const Graph &other) = default;
		Graph& operator=(const Graph &other) = default;
		Graph(Graph &&other) noexcept = default;
		Graph& operator=(Graph &&other) noexcept = default;	
		~Graph() = default;

		/* Insert and Remove Graph ADT */
		VertexID InsertVertex(const VertexData &data);
		EdgeID InsertEdge(VertexID u, VertexID v, const EdgeData &data);
		void RemoveVertex(VertexID id);
		void RemoveEdge(EdgeID id);
		void RemoveEdge(VertexID u, VertexID v);

		/* Graph Info ADT */
		bool adjacent(VertexID u, VertexID v) const;
		size_t num_vertices(void) const { return vertices_.size() - free_vertex_ids_.size(); }
		size_t num_edges(void) const { return edges_.size() - free_edge_ids_.size(); }
		std::vector<EdgeID> GetIncidentEdges(VertexID id) const;

	private:
		/* helper functions */
		inline bool vertex_exists(VertexID id) const;
		inline bool edge_exists(EdgeID id) const;

};


template<typename VertexData, typename EdgeData, bool IsDirected>
VertexID Graph<VertexData, EdgeData, IsDirected>::InsertVertex(const VertexData &data)
{
	VertexID id; /* return value */

	/* insert the node */
	if (free_vertex_ids_.empty()) {
		id = vertices_.size();
		vertices_.emplace_back(Vertex<VertexData>(id, data));
		adj_.emplace_back(std::list<AdjInfo>());
	} else {
		id = free_vertex_ids_.back();
		free_vertex_ids_.pop_back();
		vertices_[id] = Vertex<VertexData>(id, data);
		adj_[id].clear(); /* redundant actually, simply for safety */
	}

	return id;
}

/* repetition edges and self edge is allowed */
template<typename VertexData, typename EdgeData, bool IsDirected>
EdgeID Graph<VertexData, EdgeData, IsDirected>::InsertEdge(VertexID u, VertexID v, const EdgeData &data)
{
	/* safety check */
	if (!vertex_exists(u) || !vertex_exists(v)) {
		return static_cast<size_t>(-1); /* vertex doesn' t exist */
	}

	EdgeID id;

	/* insert the edge */
	if (free_edge_ids_.empty()) {
		id = edges_.size();
		edges_.emplace_back(Edge<EdgeData>(id, u, v, data));
	} else {
		id = free_edge_ids_.back();
		free_edge_ids_.pop_back();
		edges_[id] = Edge<EdgeData>(id, u, v, data);
	}

	/* update adj info */
	adj_[u].emplace_back(AdjInfo{v, id});
	edges_[id].pos_u_adj = std::prev(adj_[u].end());
	adj_[v].emplace_back(AdjInfo{u, id}); 			/* u == v is fine */
	edges_[id].pos_v_adj = std::prev(adj_[v].end());

	return id;
}

template<typename VertexData, typename EdgeData, bool IsDirected>
void Graph<VertexData, EdgeData, IsDirected>::RemoveVertex(VertexID id)
{
	if (!vertex_exists(id))	{
		return;
	}

	std::vector<EdgeID> edges(adj_[id].size());
	auto it = adj_[id].begin();
	for (size_t i = 0; i < adj_[id].size(); i++) {
		edges[i] = it->edge_id;
		++it;
	}

	for (auto &id : edges) {
		RemoveEdge(id);
	}
	
	/* remove the vertex */
	adj_[id].clear(); /* redundant actually, for safety */
	vertices_[id].is_deleted = true;
	free_vertex_ids_.emplace_back(id);
}

template<typename VertexData, typename EdgeData, bool IsDirected>
void Graph<VertexData, EdgeData, IsDirected>::RemoveEdge(EdgeID id)
{
	if (!edge_exists(id)) {
		return;
	}
	Edge<EdgeData> &edge = edges_[id];
	
	/* remove adj info */
	std::list<AdjInfo>::iterator pos_u_adj = edge.pos_u_adj;
	std::list<AdjInfo>::iterator pos_v_adj = edge.pos_v_adj;
	adj_[edge.u].erase(pos_u_adj);
	adj_[edge.v].erase(pos_v_adj);

	/* remove edge */
	edge.is_deleted = true;
	free_edge_ids_.emplace_back(id);
}

/* if repetition edge exists, it will remove all */
template<typename VertexData, typename EdgeData, bool IsDirected>
void Graph<VertexData, EdgeData, IsDirected>::RemoveEdge(VertexID u, VertexID v)
{
	if (!vertex_exists(u) || !vertex_exists(v)) {
		return;
	}

	std::vector<EdgeID> edges;
	for (auto &info : adj_[u]) {
		if (info.neighbor_id != v) {
			continue;
		}
			
		Edge<EdgeData> &edge = edges_[info.edge_id];
		if (IsDirected) {
			if (edge.u == u && edge.v == v) { /* right direction */
				edges.emplace_back(edge.id);
			}
		} else {
			edges.emplace_back(edge.id);
		}

	}

	for (auto &id : edges) {
		RemoveEdge(id);
	}
}


template<typename VertexData, typename EdgeData, bool IsDirected>
bool Graph<VertexData, EdgeData, IsDirected>::adjacent(VertexID u, VertexID v) const
{
	if (!vertex_exists(u) || !vertex_exists(v)) {
		return false;
	}

	VertexID smaller = (adj_[u].size() > adj_[v].size()) ? v : u;
	VertexID larger = (smaller == v) ? u : v;

	/* iterate the smaller one */
	for (const auto &info : adj_[smaller]) {
		if (info.neighbor_id == larger) {
			if (IsDirected) {
				const Edge<EdgeData> &edge = edges_[info.edge_id];
				if (edge.u == u && edge.v == v) {
					return true;
				}
			} else {
				return true;
			}
		}
	}

	return false;
}

template<typename VertexData, typename EdgeData, bool IsDirected>
std::vector<EdgeID> Graph<VertexData, EdgeData, IsDirected>::GetIncidentEdges(VertexID id) const
{
	if (!vertex_exists(id)) {
		return std::vector<EdgeID>();
	} else {
		std::vector<EdgeID> edges;
		for (auto it = adj_[id].begin(); it != adj_[id].end(); ++it) {
			if (IsDirected) {
				const Edge<EdgeData> &edge = edges_[it->edge_id];
				if (edge.u == id) {
					edges.emplace_back(it->edge_id);
				}
			} else {
				edges.emplace_back(it->edge_id);
			}
		}
		return edges;
	}
}

template<typename VertexData, typename EdgeData, bool IsDirected>
bool Graph<VertexData, EdgeData, IsDirected>::vertex_exists(VertexID id) const
{
	if (id >= vertices_.size()) {
		return false;
	} 
	return (vertices_[id].is_deleted == false);
}


template<typename VertexData, typename EdgeData, bool IsDirected>
bool Graph<VertexData, EdgeData, IsDirected>::edge_exists(EdgeID id) const
{
	if (id >= edges_.size()) {
		return false;
	} 
	return (edges_[id].is_deleted == false);
}

#endif /* GRAPH_HPP_ */