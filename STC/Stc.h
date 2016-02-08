#ifndef STC_H_
#define STC_H_

#include "../Map/Map.h"
#include "Node.h"

using namespace std;

// STC algorithm implementation.
// Gets a map instant and build a path that will
// cover the entire map area, considering inflation robot size etc.
class Stc {
private:

	Map &map_;
	vector<vector<Node *> > graph_;
	vector<Node *>dfs_path_;
	vector<Node*> way_points_path_;
	vector<Node*> thin_way_points_path_;

	int graph_width_;
    int graph_height_;

    //Building the Nodes graph according to the coarse grid
	void BuildGraph();
	void Dfs(Node* start_node);

	// gets the dfs path from Dfs().
	// Builds the way point vector around the dfs path.
	// Each cell the size of a robot
	void BuildWayPointVector(vector<Node *> dfs_path);

	// chooses only the corners of the path.
	void ThinWayPointsVector(vector<Node *> way_points_path);
	void AddWayPointToPath(int x1, int y1, int x2, int y2);

public:

	Stc(Map &map, Position start_position);

	unsigned int getGraphWidth();
	unsigned int getGraphHeight();

	vector<Node*>& GetPath();
	vector<Node*>& GetWayPointsPath();
	vector<Node*>& GetThinWayPointPath();
	vector<vector<Node*> > GetGraph(){return graph_;};

	virtual ~Stc();

};

#endif /*STC_H_*/

