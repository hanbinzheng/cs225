
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "imageTraversal/DFS.h"
#include "imageTraversal/BFS.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main() {

  	// @todo [Part 3]
  	// - The code below assumes you have an Animation called `animation`
  	// - The code provided below produces the `myFloodFill.png` file you must
  	//   submit Part 3 of this assignment -- uncomment it when you're ready.
 
	PNG input;
	input.readFromFile("./tests/i.png");

	/* first flood fill operation */
	FloodFilledImage ff_img(input);
	Point p1(20, 20);
    	DFS dfs(input, p1, 100);
    	MyColorPicker rand_picker;
    	ff_img.addFloodFill(dfs, rand_picker);

	/* second flood fill operation */
	Point p2(100, 100);
    	BFS bfs(input, p2, 100);
    	RainbowColorPicker rainbow_picker(5.0f);
    	ff_img.addFloodFill(bfs, rainbow_picker);	

	

	Animation animation = ff_img.animate(100);
  	PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  	lastFrame.writeToFile("myFloodFill.png");
  	animation.write("myFloodFill.gif");

  	return 0;
}
