#include "maze.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <queue>

/**
 * Makes a new SquareMaze of the given height and width.
 * If this object already represents a maze it will clear all 
 * the existing data before doing so. 
 * 
 * You will start with a square grid (like graph paper) with the 
 * specified height and width. 
 * 
 * You will select random walls to delete without creating a cycle, 
 * until there are no more walls that could be deleted without creating 
 * a cycle. Do not delete walls on the perimeter of the grid.
 * 
 * Hints: You only need to store 2 bits per square: the "down" and "right" walls. 
 * The finished maze is always a tree of corridors.)
 * 
 * @param width	The width of the SquareMaze (number of cells)
 * @param height The height of the SquareMaze (number of cells)
 */
void SquareMaze::makeMaze(int width, int height)
{
	if (width == 0 || height == 0) {
		return;
	}

	w_ = width;
	h_ = height;
	right_wall_.resize(w_ * h_, true);
	down_wall_.resize(w_ * h_, true);
	/* srand(time(NULL)); */ 
	/* this random fails, since time to generate is less than 1s */
	/* and this contains probability to generate exactly 2 same maze */
	std::random_device rd;
	std::mt19937 g(rd());

	/* kruskal to remove walls while making no cycles */
	DisjointSets dsets;
	dsets.addelements(w_ * h_);
	int walls_removed = 0;
	while (walls_removed < w_ * h_ - 1) {
        	int x = g() % w_;
        	int y = g() % h_;
        	int dir = g() % 2;

        	int idx = y * w_ + x;
        	bool removed = false;

        	if (dir == 0 && x < w_ - 1) {
            		int right_idx = y * w_ + (x + 1);
        		if (dsets.find(idx) != dsets.find(right_idx)) {
                		dsets.setunion(idx, right_idx);
        			right_wall_[idx] = false;
                		removed = true;
            		}
		} else if (dir == 1 && y < h_ - 1) {
            		int down_idx = (y + 1) * w_ + x;
            		if (dsets.find(idx) != dsets.find(down_idx)) {
                		dsets.setunion(idx, down_idx);
                		down_wall_[idx] = false;
                		removed = true;
			}
		}
		
		if (removed) walls_removed++;
	}
}


/**
 * This uses your representation of the maze to determine whether it is possible 
 * to travel in the given direction from the square at coordinates (x,y).
 * 
 * eg, after makeMaze(2,2), the possible input coordinates will be (0,0), (0,1), (1,0), and (1,1).
 * 	dir = 0 represents a rightward step (+1 to the x coordinate)
 * 	dir = 1 represents a downward step (+1 to the y coordinate)
 * 	dir = 2 represents a leftward step (-1 to the x coordinate)
 * 	dir = 3 represents an upward step (-1 to the y coordinate)
 * 
 * You can not step off of the maze or through a wall.
 * 
 * This function will be very helpful in solving the maze. 
 * It will also be used by the grading program to verify that your maze is a tree that 
 * occupies the whole grid, and to verify your maze solution. 
 * So make sure that this function works!
 * 
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @param dir The desired direction to move from the current cell
 * 
 * @return whether you can travel in the specified direction
 */
bool SquareMaze::canTravel(int x, int y, int dir) const
{
	if (x < 0 || y < 0 || x >= w_ || y >= h_) {
		return false; /* safety */
	}
	
	int idx = y * w_ + x;
	switch (dir) {
        	case 0: 
			return (x != w_ - 1) && !right_wall_[idx];
        	case 1: 
			return (y != h_ - 1) && !down_wall_[idx];
        	case 2: 
			return (x != 0) && !right_wall_[y * w_ + (x - 1)];
        	case 3: 
			return (y != 0) && !down_wall_[(y - 1) * w_ + x];
        	default: 
			return false;
    	}
}


/**
 * Sets whether or not the specified wall exists.
 * 
 * This function should be fast (constant time). 
 * You can assume that in grading we will not make your maze a non-tree and then 
 * call one of the other member functions. 
 * 
 * setWall should not prevent cycles from occurring, but should simply set a wall 
 * to be present or not present. 
 * Our tests will call setWall to copy a specific maze into your implementation.
 * 
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @param dir 	Either 0 (right) or 1 (down), which specifies which wall 
 * 		to set (same as the encoding explained in canTravel). 
 * 		You only need to support setting the bottom and right walls of every 
 * 		square in the grid.
 * @param exists true if setting the wall to exist, false otherwise
 */
void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
	if (x < 0 || y < 0 || x >= w_ || y >= h_) {
		return; /* safety */
	}
    
	int idx = y * w_ + x;
	if (dir == 0) {
		right_wall_[idx] = exists;
	} else if (dir == 1) {
		down_wall_[idx] = exists;
	}
}


/**
 * Solves this SquareMaze.
 * 
 * For each square on the bottom row (maximum y coordinate), there is a distance 
 * from the origin (i.e. the top-left cell), which is defined as the 
 * length (measured as a number of steps) of the only path through the maze from 
 * the origin to that square.
 * 
 * Select the square in the bottom row with the largest distance from the origin 
 * as the destination of the maze. solveMaze() returns the winning path from the origin 
 * to the destination as a vector of integers, where each integer represents the 
 * direction of a step, using the same encoding as in canTravel().
 * 
 * If multiple paths of maximum length exist, use the one with the destination 
 * cell that has the smallest x value.
 * 
 * Hint: this function should run in time linear in the number of cells in the maze.
 * 
 * @returns a vector of directions taken to solve the maze
 */
std::vector<int> SquareMaze::solveMaze()
{
	std::vector<int> parent(w_ * h_, -1);
	std::vector<int> dir_to_parent(w_ * h_, -1);
	std::vector<int> dist(w_ * h_, -1);
	std::queue<int> q;

	q.push(0);
	dist[0] = 0;
	int dx[] = {1, 0, -1, 0};
	int dy[] = {0, 1, 0, -1};

    	while (!q.empty()) {
        	int curr = q.front();
        	q.pop();

        	int x = curr % w_;
        	int y = curr / w_;

        	for (int d = 0; d < 4; d++) {
            		if (!canTravel(x, y, d)) 
				continue;

            		int nx = x + dx[d];
            		int ny = y + dy[d];
            		int nidx = ny * w_ + nx;
            
		
			if (dist[nidx] == -1) {
                		dist[nidx] = dist[curr] + 1;
				parent[nidx] = curr;
				dir_to_parent[nidx] = d;
				q.push(nidx);
			}
		}
	}

    	int max_dist = -1;
    	int dest = 0;
    	for (int x = 0; x < w_; x++) {
        	int y = h_ - 1;
        	int idx = y * w_ + x;
        	if (dist[idx] > max_dist) {
            		max_dist = dist[idx];
            		dest = idx;
		}
	}

	std::vector<int> path;
	while (dest != 0) {
		int d = dir_to_parent[dest];
        	path.push_back(d);
        	dest = parent[dest];
	}
	
	std::reverse(path.begin(), path.end());
	return path;
}


/**
 * Draws the maze without the solution.
 * 
 * First, create a new PNG. Set the dimensions of the PNG to (width*10+1,height*10+1),
 * where height and width were the arguments to makeMaze. 
 * 
 * Blacken the entire topmost row and leftmost column of pixels, except the 
 * entrance (1,0) through (9,0). For each square in the maze, call its maze 
 * coordinates (x,y). If the right wall exists, then blacken the pixels with 
 * coordinates ((x+1)*10,y*10+k) for k from 0 to 10. 
 * 
 * If the bottom wall exists, then blacken the pixels with coordinates (x*10+k, (y+1)*10) 
 * for k from 0 to 10.
 * 
 * The resulting PNG will look like the sample image, except there will be no exit 
 * from the maze and the red line will be missing.
 * 
 * @returns a PNG of the unsolved SquareMaze
 */
cs225::PNG *SquareMaze::drawMaze() const
{
	cs225::PNG *png = new cs225::PNG(w_ * 10 + 1, h_ * 10 + 1);

    	for (int i = 0; i < w_ * 10 + 1; i++) {
		png->getPixel(i, 0)->l = 0;
	}
	for (int i = 0; i < h_ * 10 + 1; i++) {
		png->getPixel(0, i)->l = 0;
	}
	for (int i = 1; i <= 9; i++) {
		png->getPixel(i, 0)->l = 1;
	}

    	for (int x = 0; x < w_; x++) {
		for (int y = 0; y < h_; y++) {
			int idx = y * w_ + x;
			if (right_wall_[idx]) {
				for (int k = 0; k <= 10; k++) {
					png->getPixel((x + 1) * 10, y * 10 + k)->l = 0;
				}
			} 
			if (down_wall_[idx]) {
				for (int k = 0; k <= 10; k++) {
					png->getPixel(x * 10 + k, (y + 1) * 10)->l = 0;
				}
			}
		}
	}
	
	return png;
}


/**
 * This function calls drawMaze, then solveMaze; it modifies the PNG from drawMaze to 
 * show the solution vector and the exit.
 * 
 * Start at pixel (5,5). Each direction in the solution vector corresponds to a trail 
 * of 11 red pixels in the given direction. 
 * 
 * If the first step is downward, color pixels (5,5) through (5,15) red. (Red 
 * is 0,1,0.5,1 in HSLA). 
 * Then if the second step is right, color pixels (5,15) through (15,15) red. 
 * Then if the third step is up, color pixels (15,15) through (15,5) red. 
 * Continue in this manner until you get to the end of the solution vector, so that 
 * your output looks analogous the above picture.
 * 
 * Make the exit by undoing the bottom wall of the destination square: 
 * call the destination maze coordinates (x,y), and whiten the pixels with 
 * coordinates (x*10+k, (y+1)*10) for k from 1 to 9.
 * 
 * @returns a PNG of the solved SquareMaze
 */
cs225::PNG *SquareMaze::drawMazeWithSolution()
{
	cs225::PNG *png = drawMaze();
	std::vector<int> path = solveMaze();
	
	int curr_x = 5;
	int curr_y = 5;
	
	/* beginning: red */
	cs225::HSLAPixel *start_p = png->getPixel(curr_x, curr_y);
	start_p->h = 0.0f; 
	start_p->s = 1.0f; 
	start_p->l = 0.5f; 
	start_p->a = 1.0f;

	for (int d : path) {
		/* each time, 10 forward */
		for (int i = 0; i < 10; i++) {
			switch (d) {
				case 0: curr_x++; break;
				case 1: curr_y++; break;
				case 2: curr_x--; break;
				case 3: curr_y--; break;
			}
			cs225::HSLAPixel *p = png->getPixel(curr_x, curr_y);
			p->h = 0.0;
			p->s = 1.0;
			p->l = 0.5;
			p->a = 1.0;
		}
	}
	
	int final_cell_x = (curr_x - 5) / 10;
	int final_cell_y = (curr_y - 5) / 10;
	
	/* open the final one */
	for (int k = 1; k <= 9; k++) {
		cs225::HSLAPixel *p = png->getPixel(final_cell_x * 10 + k, (final_cell_y + 1) * 10);
		p->h = 0.0f;
		p->s = 0.0f;
		p->l = 1.0f;
		p->a = 1.0f;
	}
	
	return png;
}