/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

Point<3> convertToLAB(HSLAPixel pixel) {
    Point<3> result(pixel.h/360, pixel.s, pixel.l);
    return result;
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
	map<Point<3>, int> tile_avg_map;
	for (size_t i = 0; i < theTiles.size(); ++i) {
        	Point<3> p = convertToLAB(theTiles[i].getAverageColor());
        	points.push_back(p);
        	tile_avg_map[p] = i; 
    	}

	/* construct the tree */
	KDTree<3> kdtree(points);

	TileImage *tmp = nullptr;
	for (unsigned x = 0; x < theSource.getRows(); x++) {
		for (unsigned y = 0; y < theSource.getColumns(); y++) {
			tmp = get_match_at_idx(kdtree, 
				tile_avg_map, theTiles, theSource, x, y);
			canvas->setTile(x, y, tmp);
		}
	}
	
	return canvas;
}

TileImage* get_match_at_idx(const KDTree<3>& tree,
                                  map<Point<3>, int> tile_avg_map,
                                  vector<TileImage>& theTiles,
                                  const SourceImage& theSource, int row,
                                  int col)
{
    // Create a tile which accurately represents the source region we'll be
    // using
    HSLAPixel avg = theSource.getRegionColor(row, col);
    Point<3> avgPoint = convertToLAB(avg);
    Point<3> nearestPoint = tree.findNearestNeighbor(avgPoint);

    // Check to ensure the point exists in the map
    map< Point<3>, int >::iterator it = tile_avg_map.find(nearestPoint);
    if (it == tile_avg_map.end())
        cerr << "Didn't find " << avgPoint << " / " << nearestPoint << endl;

    // Find the index
    int index = tile_avg_map[nearestPoint];
    return &theTiles[index];

}
