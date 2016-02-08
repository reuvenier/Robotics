#ifndef NODE_H_
#define NODE_H_

#include "limits.h"
#include <vector>
using namespace std;

typedef pair<int, int> Position;
const int kNumOfNeighborsInSpanningTree = 4;

// representing a node in a grid
class Node {

public:
	int row_, col_;
	bool visited_;
	bool if_exist_wall_;
	vector<Node *> neighbors_in_spanning_tree_;
	vector<Node *> convertVector2dTo1d(vector<vector<Node *> > graph);
	Node();
	Node(int row, int col);
	virtual ~Node();
	Position GetPosition();
	bool IsVisited();
	bool IsIfExistWall();
	void SetIfExistWall(bool if_exist_wall);
	bool operator<(const Node& node) const
	{
		return  (row_*INT_MAX + col_ < node.row_*INT_MAX + node.col_);
	}
};

#endif /* NODE_H_ */
