/*
 * Map.h
 *
 *  Created on: Nov 18, 2015
 *      Author: colman
 */

#ifndef MAP_H_
#define MAP_H_
#include "../STC/Node.h"
#include "lodepng.h"
#include <vector>
using namespace std;
typedef vector<vector<bool> > Grid;
class Map {
private:
	vector<unsigned char> image;

	vector<vector<Node *> > gridGraph;
	Grid map;
	Grid inflateMap;
	//original Map size
	unsigned int mapWidth;
	unsigned int mapHeight;
	//grid Map size
	unsigned int fineMapWidth;
	unsigned int fineMapHeight;
	//coarse Map size
	unsigned int coarseMapWidth;
	unsigned int coarseMapHeight;
	//Converter from coarse to grid
	int a,b;
	float mapResolution;
	float robotSize;
	//size of the robot in pixels
	int robotSizeInCells;
	//the inflation of the map
	int inflationRadius;
	Grid fineGrid; // each cell in the size of the robot
	Grid coarseGrid; // each cell in the size 2x2 of the fine grid cell

	bool checkIfCellIsOccupied(int i, int j);
	void inflate(unsigned int i ,unsigned int j);

	void drawline(int x1, int y1, int x2, int y2, int r, int g, int b);
	void addWayPoint(int x1, int y1, int x2, int y2, int r, int g, int b);

public:
	vector<unsigned char> Gridimage;
	vector<Node *>  forwardPath;
	vector<Node *>  wayPoints;
	/**
	 * constructor
	 */
	Map(float mapResolution, float robotSize);
	/**
	 * load map from file
	 */
	void loadMapFromFile(const char* filePath);
	void drawPointsInfile(char* filePath , vector<vector<Node *> > graph);
	void addDFSPathToFile(char* filePath ,vector<Node *> path);
	void addSTCPathToFile(char* filePath ,vector<Node *> path);
	Node* findNode(int x,int y);
	bool checkIfRedline(Node* point1,Node* point2);
	void inflateObstacles();
	void printMap() const;
	void printGrid(const Grid &grid) const;
	vector<vector<bool> > getInflateMap();
	void writeinflateObstacles(vector<vector<bool> > map);
	void buildFineGrid();
	void buildCoarseGrid();
	void calcGridGraphindex(int i,int j);
	void createEdgeBetweenPoints(Node* point1,Node* point2);
	virtual ~Map();

	const Grid& getFineGrid() const;
	const Grid& getCoarseGrid()const;

	const vector<vector<Node*> >& getGridGraph() const {
		return gridGraph;
	}



	void setGridGraph(const vector<vector<Node*> >& gridGraph) {
		this->gridGraph = gridGraph;
	}

	const vector<Node*>& getWayPoints() const {
		return wayPoints;
	}

	void setWayPoints(const vector<Node*>& wayPoints) {
		this->wayPoints = wayPoints;
	}
};

#endif /* MAP_H_ */
