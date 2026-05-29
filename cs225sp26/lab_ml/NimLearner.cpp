/**
 * @file NimLearner.cpp
 * CS 225 - Fall 2017
 */

#include "NimLearner.h"
#include <cstdlib>
#include <ctime>


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : 
	g_(true), total_tokens_(startingTokens), p1_(), p2_()
{
	p1_.reserve(startingTokens);
	p2_.reserve(startingTokens);
	/* v[0], v[1], v[2], v[3] ...*/
	/* p-10, p-09, p-08, p-07 ... */

	/* creates all vetices */
	for (unsigned i = 0; i <= startingTokens; ++i) {
		std::string label_p1 = std::string("p1-") + std::to_string(startingTokens - i);
		std::string label_p2 = std::string("p2-") + std::to_string(startingTokens - i);
		p1_.emplace_back(g_.insertVertex(label_p1));
		p2_.emplace_back(g_.insertVertex(label_p2));
	}

	/* create edges and assign edge weights */
	for (unsigned i = 0; i < startingTokens; ++i) {
		g_.insertEdge(p1_[i], p2_[i + 1]);			
		g_.setEdgeWeight(p1_[i], p2_[i + 1], 0);
		g_.insertEdge(p2_[i], p1_[i + 1]);
		g_.setEdgeWeight(p2_[i], p1_[i + 1], 0);
			
		if (i != startingTokens - 1) {
			g_.insertEdge(p1_[i], p2_[i + 2]);
			g_.setEdgeWeight(p1_[i], p2_[i + 2], 0);
			g_.insertEdge(p2_[i], p1_[i + 2]);
			g_.setEdgeWeight(p2_[i], p1_[i + 2], 0);
		}
	}

	startingVertex_ = p1_[0]; /* UB, the fucking stupid document never metion this */
	srand(time(nullptr)); /* set the seed for randomness */
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const 
{
	if (total_tokens_ == 0) {
		return std::vector<Edge>(); /* UB: empty case */
	}

 

	/* basic preparation */
	vector<Edge> path;
	unsigned taken = 0; /* current number of tokens = total_tokens - taken */
	bool player = true; /* true: p1, false: p2 */

	/* mimic the gaming */
	while(taken < total_tokens_) {
		unsigned tokens_left = total_tokens_ - taken;
		unsigned step = (tokens_left <= 2) ? tokens_left : (rand() % 2 + 1);
		const std::vector<Vertex> &curr = player ? p1_ : p2_; /* curr player */
		const std::vector<Vertex> &next = player ? p2_ : p1_; /* next player */
		path.emplace_back(g_.getEdge(curr[taken], next[taken + step]));
		taken += step;
		player = !player;
	}

  	return path;
}


/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) 
{
	if (path.empty()) {
		return;
	}

	// std::cout << "First Edge: " << g_.getVertexLabel(path[0].source) << " to " 
	// 	<< g_.getVertexLabel(path[0].dest) << std::endl;
	// std::cout << "Last Edge: " << g_.getVertexLabel(path.back().source) << " to " 
	// 	<< g_.getVertexLabel(path.back().dest) << std::endl;

	/* check whether the beginner win */
	Vertex beginner = path[0].source;
	Vertex winner = path.back().source;
	
	/* determine whether biginner wins */	
	int reward = 
		(g_.getVertexLabel(beginner)[1] == g_.getVertexLabel(winner)[1]) ? 1 : -1;

	/* set edge weights */
	for (const auto &edge : path) {	
		g_.setEdgeWeight(edge.source, edge.dest, 
			g_.getEdgeWeight(edge.source, edge.dest) + reward);
		reward = - reward;
	}
}


/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const 
{
	return g_;
}
