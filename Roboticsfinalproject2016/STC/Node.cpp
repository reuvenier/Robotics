/*
 * Node.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: colman
 */

#include "Node.h"

Node::Node()
{
	row = 0;
	col = 0;
	visited = false;
	ifExistWall = false;
}
Node::Node(int row, int col) : row(row), col(col), visited(false), ifExistWall(false) {
	neighborsInSpanningTree.resize(4);
}

Position Node::getPosition() {
	Position position;
    ifExistWall = false;
	position.first = row;
	position.second = col;
	return position;
}

bool Node::isIfExistWall(){
		return ifExistWall;
	}

void Node::setIfExistWall(bool ifExistWall) {
		this->ifExistWall = ifExistWall;
	}

Node::~Node() {

}

