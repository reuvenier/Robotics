#include "Stc.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

Stc::Stc(Map &map, Position start_position) :map_(map) {
	BuildGraph();
	Dfs(graph_[start_position.first][start_position.second]);
	BuildWayPointVector(dfs_path_);
	ThinWayPointsVector(way_points_path_);
}

void Stc::BuildGraph() {
	Grid coarse_grid = map_.GetCoarseGrid();
	this->graph_height_ = coarse_grid.size();
	this->graph_width_ = coarse_grid[0].size();

	graph_.resize(graph_height_);
	for (int i = 0; i < graph_height_; i++)
		graph_[i].resize(graph_width_);

	for (int i = 0; i < graph_height_; i++) {
		for (int j = 0; j < graph_width_; j++) {
			// cell is not occupied in coarseGrid
			if (!coarse_grid[i][j]) {
				Node* node = new Node(i, j);
				graph_[i][j] = node;
			}
		}
	}
}


void Stc::Dfs(Node* start_node) {
	start_node->visited_ = true;
	unsigned int x = start_node->GetPosition().first;
	unsigned int y = start_node->GetPosition().second;

	//add the point of path
	dfs_path_.push_back(start_node);

	// Up
	unsigned int row = x - 1;
	unsigned int col = y;
	if (row >= 0 && row < graph_.size() && col >= 0 && col < graph_[0].size()) {
		//check if exist node in this field
		if (graph_[row][col] != NULL && !graph_[row][col]->visited_) {
			start_node->neighbors_in_spanning_tree_[0] = graph_[row][col];
			Dfs(graph_[row][col]);
			dfs_path_.push_back(start_node);
		}
	}

	// Left
	row = x;
	col = y - 1;
	if (row >= 0 && row < graph_.size() && col >= 0 && col < graph_[0].size()) {
		if (graph_[row][col] != NULL && !graph_[row][col]->visited_) {
			start_node->neighbors_in_spanning_tree_[3] = graph_[row][col];
			Dfs(graph_[row][col]);
			dfs_path_.push_back(start_node);
		}
	}

	// Down
	row = x + 1;
	col = y;
	if (row >= 0 && row < graph_.size() && col >= 0 && col < graph_[0].size()) {
		if (graph_[row][col] != NULL && !graph_[row][col]->visited_) {
			start_node->neighbors_in_spanning_tree_[2] = graph_[row][col];
			Dfs(graph_[row][col]);
			dfs_path_.push_back(start_node);
		}
	}

	// Right
	row = x;
	col = y + 1;
	if (row >= 0 && row < graph_.size() && col >= 0 && col < graph_[0].size()) {
		if (graph_[row][col] != NULL && !graph_[row][col]->visited_) {
			start_node->neighbors_in_spanning_tree_[1] = graph_[row][col];
			Dfs(graph_[row][col]);
			dfs_path_.push_back(start_node);
		}
	}
}

void Stc::BuildWayPointVector(vector<Node *> dfs_path)
{
	 // 0 = Left , 1 = Right, 2 = Down, 3 = Up
	int direction = -1;
	int robot_size_in_cells = map_.getRobotSizeInCells();

	for (unsigned int z = 0; z < dfs_path.size()-1; z++) {
		int current_x = dfs_path[z]->GetPosition().first;
		int current_y = dfs_path[z]->GetPosition().second;

		int next_x = dfs_path[z+1]->GetPosition().first;
		int next_y = dfs_path[z+1]->GetPosition().second;

		//convert the row to the original map row that was read from the image
		next_x = ((next_x + 0.5) * robot_size_in_cells * 2);
		//convert the row to the original map column that was read from the image
		next_y = ((next_y + 0.5) * robot_size_in_cells * 2);

		////convert the row to the original map row that was read from the image
		current_x = ((current_x + 0.5) * robot_size_in_cells * 2);
		////convert the row to the original map column that was read from the image
		current_y = ((current_y + 0.5) * robot_size_in_cells * 2);

		//This is for going from left to right on the map
		if (current_y < next_y && current_x == next_x) {

			//Corner
			if (direction == 2) {

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells), current_y + (0.5 * robot_size_in_cells));
				//U turn
			} else if (direction == 0) {

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells), current_y + (0.5 * robot_size_in_cells));
			}


			AddWayPointToPath(current_x + (0.5 * robot_size_in_cells), current_y + (0.5 * robot_size_in_cells),
					next_x + (0.5 * robot_size_in_cells),next_y - (0.5 * robot_size_in_cells));

			direction = 1;

			//Right to left on the map
		} else if (current_y > next_y && current_x == next_x) {

			if (direction == 3) {

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells));

			} else if (direction == 1) {

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells));
			}


			AddWayPointToPath(current_x - (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells),
					next_x - (0.5 * robot_size_in_cells),next_y + (0.5 * robot_size_in_cells));
			direction = 0;

			//Down to up on the map
		} else if (current_y == next_y && current_x > next_x) {

			if (direction == 1) {

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells), current_y + (0.5 * robot_size_in_cells));

			} else if (direction == 2) {

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells), current_y + (0.5 * robot_size_in_cells));
			}

			AddWayPointToPath(current_x - (0.5 * robot_size_in_cells), current_y + (0.5 * robot_size_in_cells),
					next_x + (0.5 * robot_size_in_cells),next_y + (0.5 * robot_size_in_cells));

			direction = 3;

			//Up to down on the map
		} else if (current_y == next_y && current_x < next_x) {

			if (direction == 0) {

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells));

			} else if (direction == 3) {

				AddWayPointToPath(current_x + (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y + (0.5 * robot_size_in_cells),
						current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells));

				AddWayPointToPath(current_x - (0.5 * robot_size_in_cells),current_y - (0.5 * robot_size_in_cells),
						current_x + (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells));
			}

			AddWayPointToPath(current_x + (0.5 * robot_size_in_cells), current_y - (0.5 * robot_size_in_cells),
					next_x - (0.5 * robot_size_in_cells),next_y - (0.5 * robot_size_in_cells));

			direction = 2;
		}
	}
}

void Stc::AddWayPointToPath(int x1, int y1, int x2, int y2) {// x1,x2 belong to current, x2,y2 belong to nextl

	Node* node = new Node(x2, y2);
	way_points_path_.push_back(node);
}

void Stc::ThinWayPointsVector(vector<Node *> way_Points_path){

}

vector<Node*>& Stc::GetWayPointsPath(){
	return way_points_path_;
}

vector<Node*>& Stc::GetThinWayPointPath(){
	return thin_way_points_path_;
}

vector<Node*>& Stc::GetPath() {
	return dfs_path_;
}

unsigned int Stc::getGraphWidth() {
	return this->graph_width_;
}

unsigned int Stc::getGraphHeight() {
	return this->graph_height_;
}

Stc::~Stc() {

	for (int i = 0; i < graph_height_; i++) {
			for (int j = 0; j < graph_width_; j++) {
				if (graph_[i][j]) {

					delete graph_[i][j];
				}
			}
		}
}

