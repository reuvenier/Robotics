/*
 * STC.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: colman
 */

#include "STC.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

STC::STC(Map &map, Position startPos) :
		map(map) {
	buildGraph();
	printGraph();
	DFS(graph[startPos.first][startPos.second]);
	printDFS();
}

void STC::buildGraph() {
	Grid coarseGrid = map.getCoarseGrid();
	this->graphHeight = coarseGrid.size();
	this->graphWidth = coarseGrid[0].size();

	graph.resize(graphHeight);
	for (int i = 0; i < graphHeight; i++)
		graph[i].resize(graphWidth);

	for (int i = 0; i < graphHeight; i++) {
		for (int j = 0; j < graphWidth; j++) {
			// cell is not occupied in coarseGrid
			if (!coarseGrid[i][j]) {
				Node* node = new Node(i, j);
				graph[i][j] = node;
			}
		}
	}
}

void STC::printGraph() {
	int gridRows = graph.size();
	int gridCols = graph[0].size();

	for (int i = 0; i < gridRows; i++) {
		for (int j = 0; j < gridCols; j++) {
			if (graph[i][j]) {
				cout << setw(2) << i << ":" << setw(2) << j << " ";
			} else {
				cout << "  :   ";
			}
		}
		cout << endl;
	}
}

void STC::printDFS() {
	int gridRows = graph.size();
	int gridCols = graph[0].size();

	for (int i = 0; i < gridRows; i++) {
		for (int j = 0; j < gridCols; j++) {
			if (graph[i][j] != NULL) {
				for (int k = 0; k < 4; k++) {
					if (graph[i][j]->neighborsInSpanningTree[k] != NULL) {

						cout << "(" << graph[i][j]->getPosition().first << ","
								<< graph[i][j]->getPosition().second << ")";
						cout << " -> ";
						cout << "("
								<< graph[i][j]->neighborsInSpanningTree[k]->getPosition().first
								<< ","
								<< graph[i][j]->neighborsInSpanningTree[k]->getPosition().second
								<< ")" << endl;
					}
				}

			}
		}
	}
}

void STC::DFS(Node* n) {
	n->visited = true;
	unsigned int x = n->getPosition().first;
	unsigned int y = n->getPosition().second;

	//add the point of path
	Path.push_back(n);

	// right
	unsigned int row = x - 1;
	unsigned int col = y;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		//check if exist node in this field
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[0] = graph[row][col];
			DFS(graph[row][col]);
			Path.push_back(n);
		}
	}

	// down
	row = x;
	col = y - 1;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[3] = graph[row][col];
			DFS(graph[row][col]);
			Path.push_back(n);
		}
	}

	// left
	row = x + 1;
	col = y;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[2] = graph[row][col];
			DFS(graph[row][col]);
			Path.push_back(n);
		}
	}

	// up
	row = x;
	col = y + 1;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[1] = graph[row][col];
			DFS(graph[row][col]);
			Path.push_back(n);
		}
	}

	//Path.push_back(n);
}

/**
 * get the path of the robot
 */
const vector<Node*>& STC::getPath() const {
		return Path;
	}

vector<vector<Node *> > STC::getGraph() {
	return graph;
}

unsigned int STC::getGraphWidth() {
	return this->graphWidth;
}
unsigned int STC::getGraphHeight() {
	return this->graphHeight;
}

STC::~STC() {
	// TODO Auto-generated destructor stub
}

