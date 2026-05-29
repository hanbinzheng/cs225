#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <cstdlib>

#include "mosaiccanvas.h"
#include "util/util.h"
#include <unordered_map>
#include <thread>
#include <unordered_set>

using namespace std;
using namespace util;

bool MosaicCanvas::enableOutput = false;

/**
 * Constructor.
 *
 * @param theRows Number of rows to divide the canvas into
 * @param theColumns Number of columns to divide the canvas into
 */
MosaicCanvas::MosaicCanvas(int theRows, int theColumns)
    : rows(theRows), columns(theColumns)
{
    if ((theRows < 1) || (theColumns < 1)) {
        cerr << "Error: Cannot set non-positive rows or columns" << endl;
        exit(-1);
    }

    myImages.resize(rows * columns);
}

/**
 * Retrieve the number of rows of images
 *
 * @return The number or rows in the mosaic, or -1 on error
 */
int MosaicCanvas::getRows() const
{
    return rows;
}

/**
 * Retrieve the number of columns of images
 *
 * @return The number of columns in the mosaic, or -1 or error
 */
int MosaicCanvas::getColumns() const
{
    return columns;
}

void MosaicCanvas::setTile(int row, int column, TileImage* i)
{
    if (enableOutput) {
	#ifndef MULTI_THREADS
        	cerr << "\rPopulating Mosaic: setting tile ("
             	<< row << ", " << column
             	<< ")" << string(20, ' ') << "\r";
        	cerr.flush();
	#endif /* MULTI_THREADS */
    }
    myImages[row * columns + column] = i;
}

const TileImage& MosaicCanvas::getTile(int row, int column)
{
    return *myImages[row * columns + column];
}

PNG MosaicCanvas::drawMosaic(int pixelsPerTile)
{
	if (pixelsPerTile <= 0) {
		cerr << "ERROR: pixelsPerTile must be > 0" << endl;
		exit(-1);
	}

	int width = columns * pixelsPerTile;
	int height = rows * pixelsPerTile;
	PNG mosaic(width, height);
	std::unordered_map<TileImage *, PNG> tile_to_resized;
	tile_to_resized.reserve(columns * rows);

	#ifdef MULTI_THREADS
		/* preparation */
		unsigned num_threads = std::thread::hardware_concurrency();
		if (num_threads >= MAX_THREADS) {
			num_threads = MAX_THREADS;
		} else if (num_threads == 0) {
			num_threads = 1;
		}
		std::vector<std::thread> workers;

		/* collect unique tiles */
		std::unordered_set<TileImage *> unique;
		for (auto &t : myImages) {
			unique.insert(t);
		}
		std::vector<TileImage *> unique_tiles(unique.begin(), unique.end());

		/* define mapping task */
		auto mapping_task = [&] (int idx_start, int idx_end) {
			TileImage *curr_tile = nullptr;
			PNG map_png, map_cropped;
			PNG map_resized(pixelsPerTile, pixelsPerTile); /* the size must be correct */
			for (int x = idx_start; x < idx_end; x++) {
				curr_tile = unique_tiles[x];
				map_png.readFromFile(curr_tile->getFileName());
				map_cropped = curr_tile->cropSourceImage(map_png);
				curr_tile->generateResizedImage(
					curr_tile->getResolution(), map_cropped, map_resized);
				tile_to_resized[curr_tile] = map_resized;
			}
		};

		/* go work!! construct the map */
		int batch_size = unique_tiles.size() / num_threads;
		for (unsigned i = 0; i < num_threads; i++) {
			int start = i * batch_size;
			int end = (i == num_threads - 1) ? unique_tiles.size() : start + batch_size;
			workers.emplace_back(mapping_task, start, end);
		}
		for (auto &t : workers) {
			t.join();
		}

		/* define drawing task */
		auto drawing_task = [&] (int row_start, int row_end){
			TileImage *curr_tile;
			for (int x = row_start; x < row_end; x++) {
				for (int y = 0; y < columns; y++) {
					curr_tile = myImages[x * columns + y];
					PNG const &resized = tile_to_resized[curr_tile];

					int startX = divide(width * y, columns);
					int endX   = divide(width * (y + 1), columns);
					int startY = divide(height * x, rows);
					int endY   = divide(height * (x + 1), rows);

					if (endX - startX != endY - startY) {
						; /* error case */
					}
					curr_tile->paste(mosaic, startX, startY, resized);
				}
			}
		};

		/* go work!! drawing to the canvas! */
		workers.clear();
		int row_per_thread = rows / num_threads;
		for (unsigned i = 0; i < num_threads; i++) {
			int start = i * row_per_thread;
			int end = (i == num_threads - 1) ? rows : start + row_per_thread;
			workers.emplace_back(drawing_task, start, end);
		}
		for (auto &t : workers) {
			t.join();
		}
	#else
		TileImage *curr_tile = nullptr;
		PNG png, cropped;
		PNG resized = PNG(pixelsPerTile, pixelsPerTile); /* care about the size of resized! */
	
		for (int row = 0; row < rows; row++) {
			/* logging info */
			if (enableOutput) {
				cerr << "\rDrawing Mosaic: resizing tiles ("
                 		<< (row * columns + /*col*/ 0 + 1) << "/" << (rows * columns)
                 		<< ")" << string(20, ' ') << "\r";
            			cerr.flush();
			}

			for (int col = 0; col < columns; col++) {
				/* get the corresponding tiles */
				curr_tile = myImages[row * columns + col];

				/* if new tile, store it to the unordered map */
				if (tile_to_resized.find(curr_tile) == tile_to_resized.end()) {
					/* process the image */
					png.readFromFile(curr_tile->getFileName()); /* load image */
					cropped = curr_tile->cropSourceImage(png); /* crop image */
					curr_tile->generateResizedImage( /* resized the image */
						curr_tile->getResolution(), cropped, resized);
					tile_to_resized[curr_tile] = resized;
				} else {
					resized = tile_to_resized[curr_tile];
				}
		
				int startX = divide(width * col, columns);
				int endX   = divide(width * (col + 1), columns);
				int startY = divide(height * row, rows);
				int endY   = divide(height * (row + 1), rows);
				if (endX - startX != endY - startY) {
					cerr << "Error: resolution not constant: x: " 
					<< (endX - startX)
                     			<< " y: " << (endY - startY) << endl;
				}
				myImages[row * columns + col]->paste(mosaic, startX, startY, resized);
			}
		}
	
		if (enableOutput) {
        		cerr << "\r" << string(60, ' ');
        		cerr << "\rDrawing Mosaic: resizing tiles ("
             		<< (rows * columns) << "/" << (rows * columns) << ")" << endl;
        		cerr.flush();
    		}
	#endif /* MULTI_THREADS */

    	return mosaic;
}
