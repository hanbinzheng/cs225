/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include "maptiles.h"
#include "tools.h"
#include <iostream>
#include <map>
#include <unordered_map>
#include <thread>

using namespace std;

static inline TileImage *get_match_idx(const KDTree<3> &tree,
						const unordered_map<Point<3>, int, PointHash> &tile_avg_map,
						vector<TileImage>& theTiles,
						const SourceImage& theSource, 
						int row, int col)
{
	
    // Create a tile which accurately represents the source region we'll be
    HSLAPixel avg = theSource.getRegionColor(row, col);
    Point<3> avgPoint = convertToLAB(avg);
    Point<3> nearestPoint = tree.findNearestNeighbor(avgPoint);

    // Check to ensure the point exists in the map
    unordered_map<Point<3>, int, PointHash>::const_iterator it = tile_avg_map.find(nearestPoint);
    if (it == tile_avg_map.end())
        cerr << "Didn't find " << avgPoint << " / " << nearestPoint << endl;

    // Find the index
    int index = it->second;
    return &theTiles[index];

}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
	/**
	 * @todo Implement this function!
	 */
	MosaicCanvas *canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

	/* prepare for datas */
	vector<Point<3>> points;
	unordered_map<Point<3>, int, PointHash> tile_avg_map;
	for (size_t i = 0; i < theTiles.size(); ++i) {
        	Point<3> p = convertToLAB(theTiles[i].getAverageColor());
        	points.push_back(p);
        	tile_avg_map[p] = i; 
    	}

	KDTree<3> kdtree(points);

	#ifdef MULTI_THREADS
		/* prepare for the multi thread case */
		unsigned num_threads = std::thread::hardware_concurrency(); /* number of threads */
		if (num_threads >= MAX_THREADS) {
			num_threads = MAX_THREADS;
		} else if (num_threads == 0) {
			num_threads = 1; /* back to the single thread case */
		}
		vector<std::thread> workers;
		int rows = theSource.getRows();
		int rows_per_thread = rows / num_threads;

		/* define task for a single thread */
		auto task = [&](int row_start, int row_end) {
			TileImage *tmp = nullptr;
			for (int x = row_start; x < row_end; x++) { 
				for (int y = 0; y < theSource.getColumns(); y++) {
					tmp = get_match_idx(kdtree, 
						tile_avg_map, theTiles, theSource, x, y);
					canvas->setTile(x, y, tmp);
				}
			}
		};

		/* assign task and join */
		for (unsigned i = 0; i < num_threads; i++) {
			int row_start = i * rows_per_thread;
			int row_end = (i == num_threads - 1) ? rows : row_start + rows_per_thread;
			workers.emplace_back(task, row_start, row_end);
		}

		/* go work!!! */
		for (auto& t : workers) {
        		t.join();
    		}
	#else
		TileImage *tmp = nullptr;
		for (int x = 0; x < theSource.getRows(); x++) {
			for (int y = 0; y < theSource.getColumns(); y++) {
				tmp = get_match_idx(kdtree, 
					tile_avg_map, theTiles, theSource, x, y);
				canvas->setTile(x, y, tmp);
			}
		}
	#endif /* MULTI_THREADS */
	
	return canvas;
}


/* old fucking stupid function */
// TileImage* get_match_at_idx(const KDTree<3>& tree,
//                                   map<Point<3>, int> tile_avg_map,
//                                   vector<TileImage>& theTiles,
//                                   const SourceImage& theSource, int row,
//                                   int col)
// {
//     // Create a tile which accurately represents the source region we'll be
//     // using
//     HSLAPixel avg = theSource.getRegionColor(row, col);
//     Point<3> avgPoint = convertToLAB(avg);
//     Point<3> nearestPoint = tree.findNearestNeighbor(avgPoint);

//     // Check to ensure the point exists in the map
//     map< Point<3>, int >::iterator it = tile_avg_map.find(nearestPoint);
//     if (it == tile_avg_map.end())
//         cerr << "Didn't find " << avgPoint << " / " << nearestPoint << endl;

//     // Find the index
//     int index = tile_avg_map[nearestPoint];
//     return &theTiles[index];

// }
