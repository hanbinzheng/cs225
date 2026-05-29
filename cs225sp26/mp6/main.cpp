#include <iostream>
#include <set>
#include <vector>
#include <chrono>

#include "cs225/PNG.h"
#include "maptiles.h"
#include "mosaiccanvas.h"
#include "sourceimage.h"
#include "util/util.h"
#include <thread>

using namespace std;
using namespace util;
using namespace cs225;

void makePhotoMosaic(const string& inFile, const string& tileDir, int numTiles,
                     int pixelsPerTile, const string& outFile);
vector<TileImage> getTiles(string tileDir);
bool hasImageExtension(const string& fileName);

namespace opts
{
    bool help = false;
}

int main(int argc, const char** argv)
{
    string inFile = "";
    string tileDir = "mp5_pngs/";
    string numTilesStr = "100";
    string pixelsPerTileStr = "50";
    string outFile = "mosaic.png";

    OptionsParser optsparse;
    optsparse.addArg(inFile);
    optsparse.addArg(tileDir);
    optsparse.addArg(numTilesStr);
    optsparse.addArg(pixelsPerTileStr);
    optsparse.addArg(outFile);
    optsparse.addOption("help", opts::help);
    optsparse.addOption("h", opts::help);
    optsparse.parse(argc, argv);

    if (opts::help) {
        cout << "Usage: " << argv[0]
             << " background_image.png tile_directory/ [number of tiles] "
                "[pixels per tile] [output_image.png]"
             << endl;
        return 0;
    }

    if (inFile == "") {
        cout << "Usage: " << argv[0]
             << " background_image.png tile_directory/ [number of tiles] "
                "[pixels per tile] [output_image.png]"
             << endl;
        return 1;
    }

    makePhotoMosaic(inFile, tileDir, lexical_cast<int>(numTilesStr),
                    lexical_cast<int>(pixelsPerTileStr), outFile);

    return 0;
}

void makePhotoMosaic(const string& inFile, const string& tileDir, int numTiles,
                     int pixelsPerTile, const string& outFile)
{
    	/* part 1: loading source image */
    	auto t_source_start = chrono::steady_clock::now(); 
    	PNG inImage;
    	inImage.readFromFile(inFile);
    	SourceImage source(inImage, numTiles); /* process the source image */
    	auto t_source_end = chrono::steady_clock::now();
    	double t_source_ms = chrono::duration<double, milli>(t_source_end - t_source_start).count();
    	cerr << "Loading Source Image, costing " <<  t_source_ms << " ms." << std::endl;

    	/* part2: loading time images */
    	auto t_tile_start = chrono::steady_clock::now(); 
    	vector<TileImage> tiles = getTiles(tileDir); /* process tile images */
    	auto t_tile_end = chrono::steady_clock::now(); 
    	double t_tile_ms = chrono::duration<double, milli>(t_tile_end - t_tile_start).count();
    	cerr << "Loading Tile Images, costing " <<  t_tile_ms << " ms." << std::endl;
    	if (tiles.empty()) {
        	cerr << "ERROR: No tile images found in " << tileDir << endl;
        	exit(2);
    	}

    	/* part 3: maptiles */
    	MosaicCanvas::enableOutput = true;
    	auto t_maptile_start = chrono::steady_clock::now(); 
    	MosaicCanvas* mosaic = mapTiles(source, tiles);
    	auto t_maptile_end = chrono::steady_clock::now(); 
    	double t_maptile_ms = chrono::duration<double, milli>(t_maptile_end - t_maptile_start).count();
    	#ifndef MULTI_THREADS
    		cerr << endl;
    	#endif /* MULTI_THREADS */
    	cerr << "Maptiles, costing " <<  t_maptile_ms << " ms." << std::endl;
    	if (mosaic == NULL) {
        	cerr << "ERROR: Mosaic generation failed" << endl;
        	exit(3);
    	}

    	/* part 4: draw mosic */
    	auto t_draw_start = chrono::steady_clock::now(); 
    	PNG result = mosaic->drawMosaic(pixelsPerTile);
    	auto t_draw_end = chrono::steady_clock::now(); 
    	double t_draw_ms = chrono::duration<double, milli>(t_draw_end - t_draw_start).count();
    	cerr << "Draw Mosic, costing: " << t_draw_ms << " ms." << std::endl;

    	/* part 5: save image to the disk */
    	cerr << "Saving Output Image... ";
    	auto t_write_start = chrono::steady_clock::now(); 
    	result.writeToFile(outFile);
    	auto t_write_end = chrono::steady_clock::now(); 
    	double t_write_ms = chrono::duration<double, milli>(t_write_end - t_write_start).count();
    	cerr << "Save Image, costing: " << t_write_ms << " ms." << std::endl;
    	cerr << "Done" << endl;
    	delete mosaic;
}

vector<TileImage> getTiles(string tileDir)
{
#if 1
    	if (tileDir[tileDir.length() - 1] != '/') {
		tileDir += '/';
	}

    	vector<string> allFiles = get_files_in_dir(tileDir);
    	sort(allFiles.begin(), allFiles.end());

    	vector<string> imageFiles;
    	imageFiles.reserve(allFiles.size());
    	for (size_t i = 0; i < allFiles.size(); i++) {
        	if (hasImageExtension(allFiles[i]))
            	imageFiles.push_back(allFiles[i]);
	}
	
    	vector<TileImage> images;
    	set<HSLAPixel> avgColors;

	#ifdef MULTI_THREADS
		/* preparation */
		unsigned num_threads = std::thread::hardware_concurrency();
		if (num_threads >= MAX_THREADS) {
			num_threads = MAX_THREADS;
		} else if (num_threads == 0) {
			num_threads = 1;
		}
		vector<std::thread> workers;
		vector<vector<TileImage>> thread_results(num_threads);

		/* define the load task function */
		auto task = [&](int start, int end, int thread_id) {
			for (int i = start; i < end; i++) {
				/* incorrect practice: multi-copy! */
				// TileImage img(imageFiles[i]);
				// thread_results[thread_id].push_back(img);
				thread_results[thread_id].emplace_back(imageFiles[i]);
			}
		};

		/* go work!!!! */
		int tiles_a_thread = imageFiles.size() / num_threads;
		for (unsigned i = 0; i < num_threads; i++) {
			int start = i * tiles_a_thread;
			int end = (i == num_threads - 1) ? imageFiles.size() : start + tiles_a_thread;
			workers.emplace_back(task, start, end, i);
		}
		for (auto &t : workers) {
			t.join();
		}

		/* remove repetition */
		for (auto &imgs : thread_results) {
			for (auto &img : imgs) {
				if (avgColors.count(img.getAverageColor()) == 0) {
					/* doesn' t exit, unique/first */
					avgColors.insert(img.getAverageColor());
					images.push_back(std::move(img)); /* avoid copy */
				}
			}
		}
	#else
    		for (size_t i = 0; i < imageFiles.size(); i++) {
        		cerr << "\rLoading Tile Images... ("
             		<< (i + 1) << "/" << imageFiles.size()
             		<< ")" << string(20, ' ') << "\r";
        		cerr.flush();
        		TileImage next(imageFiles.at(i));
	
        		if (avgColors.count(next.getAverageColor()) == 0) {
            			avgColors.insert(next.getAverageColor());
            			images.push_back(next);
        		}
    		}
    	
		cerr << "\rLoading Tile Images... ("
		<< imageFiles.size() << "/" << imageFiles.size()
        	<< ")";
    		cerr << "... " << images.size() << " unique images loaded" << endl;
    		cerr.flush();
	#endif /* MULTI_THREADS */

    	return images;
#else
    	PNG temp;
    	vector<TileImage> tiles;
    	*temp(0, 0) = RGBAPixel(0,   0,   0);   tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(0,   0,   127); tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(0,   0,   255); tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(0,   127, 0);   tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(0,   255, 0);   tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(127, 0,   0);   tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(255, 0,   0);   tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(0,   127, 127); tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(0,   255, 255); tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(127, 0,   127); tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(255, 0,   255); tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(127, 127, 0);   tiles.push_back(TileImage(temp));
    	*temp(0, 0) = RGBAPixel(255, 255, 0);   tiles.push_back(TileImage(temp));
    	return tiles;
#endif
}

bool hasImageExtension(const string& fileName)
{
    	size_t dotpos = fileName.find_last_of(".");
    	if (dotpos == string::npos)
        	return false;
    	string ext = toLower(fileName.substr(dotpos + 1));
    	return (ext == "bmp" || ext == "png" || ext == "jpg" || ext == "gif" || ext == "tiff");
}
