#ifndef MAZE_H_
#define MAZE_H_

#include "dsets.h"
#include "cs225/PNG.h"
#include <vector>

using namespace cs225;
using namespace std;

/* for detailed comments, see maze.cpp */
class SquareMaze 
{
	public:
		SquareMaze() = default;
		void makeMaze(int width, int height);
		bool canTravel(int x, int y, int dir) const;
		void setWall(int x, int y, int dir, bool exists);
		std::vector<int> solveMaze();
		cs225::PNG *drawMaze() const;
		cs225::PNG *drawMazeWithSolution();

	private:
		int w_; /* width */
    		int h_; /* height */
		std::vector<bool> right_wall_;
		std::vector<bool> down_wall_;

    int index(int x, int y) const;
};

#endif /* MAZE_H_ */