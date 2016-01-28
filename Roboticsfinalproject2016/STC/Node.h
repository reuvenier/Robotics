/*
 * Node.h
 *
 *  Created on: Dec 10, 2015
 *      Author: colman
 */

#ifndef NODE_H_
#define NODE_H_
#include "limits.h"

#include <vector>
using namespace std;

typedef pair<int, int> Position;

class Node {

public:
	int row, col;
	bool visited;
	bool ifExistWall;
	vector<Node *> neighborsInSpanningTree;
	vector<Node *> convertVector2dTo1d(vector<vector<Node *> > graph);
	Node();
	Node(int row, int col);
	virtual ~Node();
	Position getPosition();
	bool isVisited();
	bool isIfExistWall();
	void setIfExistWall(bool ifExistWall);
	bool operator<(const Node& node) const
	{
		return  (row*INT_MAX + col < node.row*INT_MAX + node.col);
	}
};

#endif /* NODE_H_ */
