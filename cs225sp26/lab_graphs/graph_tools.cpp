/**
 * @file graph_tools.cpp
 * This is where you will implement several functions that operate on graphs.
 * Be sure to thoroughly read the comments above each function, as they give
 *  hints and instructions on how to solve the problems.
 */

#include "graph_tools.h"

/**
 * Finds the minimum edge weight in the Graph graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @return the minimum weighted edge
 *
 * @todo Label the minimum edge as "MIN". It will appear blue when
 *  graph.savePNG() is called in minweight_test.
 *
 * @note You must do a traversal.
 * @note You may use the STL stack and queue.
 * @note You may assume the graph is connected.
 *
 * @hint Initially label vertices and edges as unvisited.
 */
int GraphTools::findMinWeight(Graph& graph)
{
	/* get all edges */
	std::vector<Edge> edges = graph.getEdges();
	if (edges.empty()) {
		return -114514; /* Undefined Behavior */
	}

	/* find the minimal weight */
	std::vector<size_t> idx_min = {0};
	int weight_min = edges[0].weight;
	for (size_t i = 0; i < edges.size(); i++) {
		Edge edge = edges[i];
		if (edge.weight < weight_min) {
			weight_min = edge.weight;
			idx_min.clear();
			idx_min.emplace_back(i);
		} else if (edge.weight == weight_min) {
			idx_min.emplace_back(i);
		}
		graph.setEdgeLabel(edge.source, edge.dest, "UNEXPLORED");
	}

	/* label the min */
	for (size_t &idx : idx_min) {
		Edge edge = edges[idx];
		graph.setEdgeLabel(edge.source, edge.dest, std::string("MIN"));
	}

	return weight_min;
}

/**
 * Returns the shortest distance (in edges) between the Vertices
 *  start and end.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @param start - the vertex to start the search from
 * @param end - the vertex to find a path to
 * @return the minimum number of edges between start and end
 *
 * @todo Label each edge "MINPATH" if it is part of the minimum path
 *
 * @note Remember this is the shortest path in terms of edges,
 *  not edge weights.
 * @note Again, you may use the STL stack and queue.
 * @note You may also use the STL's unordered_map, but it is possible
 *  to solve this problem without it.
 *
 * @hint In order to draw (and correctly count) the edges between two
 *  vertices, you'll have to remember each vertex's parent somehow.
 */
int GraphTools::findShortestPath(Graph& graph, Vertex start, Vertex end)
{
	/* initialized all vertices and edges to be unexplored */
	std::vector<Vertex> vertices = graph.getVertices();	
	for (Vertex v : vertices) {
        	graph.setVertexLabel(v, "UNEXPLORED");
    	}
	std::vector<Edge> edges = graph.getEdges();
    	for (Edge e : edges) {
        	graph.setEdgeLabel(e.source, e.dest, "UNEXPLORED");
    	}

	/* preperation for BFS */
	std::queue<Vertex> q;
	Vertex curr = start;
	std::unordered_map<Vertex, Vertex> parent; /* key: curr, val: pre of curr */
	graph.setVertexLabel(start, "VISITED");
	q.push(start);

	/* BFS finding the shortest */
	while (!q.empty()) {
		/* get the current vertex and determine whether find */
		curr = q.front();
		q.pop();
		if (curr == end)
			break;

		/* traverse all neighbors of q */
		std::vector<Vertex> neighbors = graph.getAdjacent(curr);
		for (Vertex &v : neighbors) {
			if (graph.getVertexLabel(v) == "UNEXPLORED") {
				graph.setVertexLabel(v, "VISITED");
				q.push(v);
				parent[v] = curr;
			}
		}
	}

	if (curr != end) {
		/* the path is not found */
		return -1; /* Undefined Behavior */
	}

	/* label the shorted path and calculate pathlength */
	int dist = 0;
    	curr = end; /* actually curr == end, this is only a reminder */
	while (curr != start) {
        	Vertex p = parent[curr];
        	graph.setEdgeLabel(p, curr, "MINPATH");
        	dist++;
        	curr = p;
    	}

	return dist;
}

/**
 * Finds a minimal spanning tree on a graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to find the MST of
 *
 * @todo Label the edges of a minimal spanning tree as "MST"
 *  in the graph. They will appear blue when graph.savePNG() is called.
 *
 * @note Use your disjoint sets class from MP 7.1 to help you with
 *  Kruskal's algorithm. Copy the files into the libdsets folder.
 * @note You may call std::sort instead of creating a priority queue.
 */
void GraphTools::findMST(Graph& graph)
{
	/* make basic preparations */
	std::vector<Vertex> vertices = graph.getVertices();
	if (vertices.empty()) 
		return;
	std::vector<Edge> edges = graph.getEdges();
	std::sort(edges.begin(), edges.end());

	DisjointSets disjoint_set;
	disjoint_set.addelements(vertices.size());

	size_t edge_count = 0;
	size_t final_edge = vertices.size() - 1;

	/* MST algorithm: greedy algorithm */
	for (Edge &edge : edges) {
		if (edge_count == final_edge)
			break;

		Vertex source = edge.source;
        	Vertex dest = edge.dest;

		if (disjoint_set.find(source) != disjoint_set.find(dest)) {
			graph.setEdgeLabel(source, dest, "MST");
			disjoint_set.setunion(source, dest);
			edge_count++;
		}
	}
}

