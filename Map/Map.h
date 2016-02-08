#ifndef MAP_H_
#define MAP_H_

#include <iostream>
#include "lodepng.h"
#include "../STC/Node.h"
#include <vector>

using namespace std;

typedef vector<vector<bool> > Grid;

// Responsible for the building process of the map as well as saving
// the inflated map.
class Map {

private:

	Grid map_;         // Map, not inflated
	Grid inflate_map_;  // Map, inflated
	Grid fine_grid_;    // Each cell the size of 1 robot
	Grid coarse_grid_;  // Each cell the size of 4 robots

	vector<unsigned char> grid_image_;    // The raw image from lodepng.h decode function
	vector<vector<Node *> > grid_graph_;  // Fine grid represented with Node

	unsigned int fine_map_width_;
	unsigned int fine_map_height_;

	unsigned int coarse_map_width_;
	unsigned int coarse_map_height_;

	unsigned int map_width_;
	unsigned int map_height_;

	float map_resolution_;
	float robot_size_;
	int robot_size_in_cells_;
	int inflation_radius_;

	bool CheckIfCellIsOccupied(int i, int j);

	//inflate the cell at index i,j according to the inflation radius.
	void Inflate(unsigned int i ,unsigned int j);

	//Inflates the map by iterating the map and calling Inflate()
	void InflateObstacles();
	void BuildFineGrid();
	void BuildCoarseGrid();

public:

	Map(float map_resolution, float robot_size, const char* file_path);
	void LoadMapFromFile(const char* file_path);
	void AddPathToFile(const char* file_path, vector<vector<Node*> > graph, int width, int height);
	const Grid& GetInflateMap() const;
	const Grid& GetFineGrid() const;
	const Grid& GetCoarseGrid()const;

	unsigned int getCoarseMapHeight() const {
		return coarse_map_height_;
	}

	unsigned int getCoarseMapWidth() const {
		return coarse_map_width_;
	}

	unsigned int getFineMapHeight() const {
		return fine_map_height_;
	}

	unsigned int getFineMapWidth() const {
		return fine_map_width_;
	}

	int getInflationRadius() const {
		return inflation_radius_;
	}

	unsigned int getMapHeight() const {
		return map_height_;
	}

	float getMapResolution() const {
		return map_resolution_;
	}

	unsigned int getMapWidth() const {
		return map_width_;
	}

	float getRobotSize() const {
		return robot_size_;
	}

	int getRobotSizeInCells() const {
		return robot_size_in_cells_;
	}

	virtual ~Map();
};

#endif /* MAP_H_ */
