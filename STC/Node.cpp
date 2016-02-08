/*
 * Node.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: colman
 */

#include "Node.h"

Node::Node()
{
	row_ = 0;
	col_ = 0;
	visited_ = false;
	if_exist_wall_ = false;
}
Node::Node(int row, int col) : row_(row), col_(col), visited_(false), if_exist_wall_(false) {
	neighbors_in_spanning_tree_.resize(kNumOfNeighborsInSpanningTree);
}

Position Node::GetPosition() {
	Position position;
	if_exist_wall_ = false;
	position.first = row_;
	position.second = col_;
	return position;
}

bool Node::IsIfExistWall(){
		return if_exist_wall_;
	}

void Node::SetIfExistWall(bool if_exist_wall) {
		this->if_exist_wall_ = if_exist_wall;
	}

Node::~Node() {

}

