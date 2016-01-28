#include "Map.h"
#include <iostream>

Map::Map(float mapResolution, float robotSize): mapResolution(mapResolution), robotSize(robotSize) {

	a = 0;
	b = 0;
	mapWidth = 0;
	mapHeight = 0;
	fineMapWidth = 0;
	fineMapHeight = 0;
	coarseMapWidth = 0;
	coarseMapHeight = 0;

	robotSizeInCells = robotSize / mapResolution;
	inflationRadius = 0.3 * robotSizeInCells;
	cout << "inflation radius " << inflationRadius << endl;

}

void Map::loadMapFromFile(const char* filePath) {
	lodepng::decode(image, mapWidth, mapHeight, filePath);
	fineMapWidth = mapWidth / robotSizeInCells;
	fineMapHeight = mapHeight / robotSizeInCells;
	//create grid graph for to storage the path
	gridGraph.resize(fineMapHeight);
	for (unsigned int k = 0; k < fineMapHeight; k++) {
		gridGraph[k].resize(fineMapWidth);
	}
	cout << "map size: " << mapWidth << "," << mapHeight << endl;

	map.resize(mapHeight);
	for (unsigned int i = 0; i < mapHeight; i++)
		map[i].resize(mapWidth);

	for (unsigned int i = 0; i < mapHeight; i++) {
		for (unsigned int j = 0; j < mapWidth; j++) {
			map[i][j] = checkIfCellIsOccupied(i, j);
		}
	}

}

bool Map::checkIfCellIsOccupied(int i, int j) {
	int c = (i * mapWidth + j) * 4;
	int r = image[c];
	int g = image[c + 1];
	int b = image[c + 2];

	if (r == 0 && g == 0 && b == 0)
		return true;
	return false;
}

void Map::printMap() const {
	for (unsigned int i = 0; i < mapHeight; i++) {
		for (unsigned int j = 0; j < mapWidth; j++) {
			cout << (inflateMap[i][j] ? "*" : " ");
		}
		cout << endl;
	}
}

void Map::printGrid(const Grid &grid) const {
	int rows = grid.size();
	int cols = grid[0].size();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << (grid[i][j] ? "*" : " ");
		}
		cout << endl;
	}

	rows = coarseGrid.size();
	cols = coarseGrid[0].size();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << (coarseGrid[i][j] ? "*" : " ");
		}
		cout << endl;
	}

}

void Map::inflateObstacles() {

	inflateMap.resize(mapHeight);
	for (unsigned int i = 0; i < mapHeight; i++) {
		inflateMap[i].resize(mapWidth);
	}

	for (unsigned int i = 0; i < mapHeight; i++) {
		for (unsigned int j = 0; j < mapWidth; j++) {
			if (map[i][j]) {
				inflate(i, j);
			}
		}
	}

}

void Map::inflate(unsigned int i,unsigned int j) {

	for (unsigned int k = i - inflationRadius; k < i + inflationRadius; k++) {
		for (unsigned int m = j - inflationRadius; m < j + inflationRadius; m++) {
			if (m < 0 || k < 0 || k >= mapHeight || m >= mapWidth) {
				continue;
			} else {
				// Mark this cell as occupied in inflateMap
				inflateMap[k][m] = true;
				// Draw black color in corresponding image
				int c = (k * mapWidth + m) * 4;
				image[c] = 0;
				image[c + 1] = 0;
				image[c + 2] = 0;

			}
		}
	}
}

vector<vector<bool> > Map::getInflateMap() {
	return inflateMap;
}

void Map::writeinflateObstacles(vector<vector<bool> > map) {

	lodepng::encode("robotic2.png", image, mapWidth, mapHeight);
}

void Map::buildFineGrid() {

	int fineGridCells = mapHeight / robotSizeInCells;
	int fineGridWidth = mapWidth / robotSizeInCells;
	fineGrid.resize(fineGridCells);
	for (int i = 0; i < fineGridCells; i++) {
		fineGrid[i].resize(fineGridWidth);
	}

	int flag = false;

	for (int i = 0; i < fineGridCells; i++) {
		for (int j = 0; j < fineGridWidth; j++) {
			flag = false;
			for (int m = i * robotSizeInCells;
					m < i * robotSizeInCells + robotSizeInCells; m++) {
				for (int z = j * robotSizeInCells;
						z < j * robotSizeInCells + robotSizeInCells; z++) {
					if (inflateMap[m][z] == true) {
						flag = true;
						break;
					}
				}
			}
			fineGrid[i][j] = flag;
		}
	}

}

void Map::buildCoarseGrid() {

	int fineGridCells = mapHeight / (robotSizeInCells * 2);
	coarseMapHeight = fineGridCells;
	int fineGridWidth = mapWidth / (robotSizeInCells * 2);
	coarseMapWidth = fineGridWidth;
	coarseGrid.resize(fineGridCells);
	for (int i = 0; i < fineGridCells; i++) {
		coarseGrid[i].resize(fineGridWidth);
	}

	int fleg = false;

	for (int i = 0; i < fineGridCells; i++) {
		for (int j = 0; j < fineGridWidth; j++) {
			fleg = false;
			for (int m = i * (robotSizeInCells * 2);
					m < i * (robotSizeInCells * 2) + (robotSizeInCells * 2);
					m++) {
				for (int z = j * (robotSizeInCells * 2);
						z < j * (robotSizeInCells * 2) + (robotSizeInCells * 2);
						z++) {
					if (inflateMap[m][z] == true) {
						fleg = true;
						break;
					}
				}
			}
			coarseGrid[i][j] = fleg;
		}
	}

}

void Map::addDFSPathToFile(char* filePath, vector<Node *> path) {
	for (unsigned int z = 0; z < path.size(); z++) {
		for (int k = 0; k < 4; k++) {
			if (path[z]->neighborsInSpanningTree[k] != NULL) {
				int i = path[z]->getPosition().first;
				int j = path[z]->getPosition().second;
				//convert the row to the original map row that was read from the image
				i = ((i + 0.5) * robotSizeInCells * 2);
				//convert the row to the original map column that was read from the image
				j = ((j + 0.5) * robotSizeInCells * 2);

				int i1 =
						path[z]->neighborsInSpanningTree[k]->getPosition().first;
				int j1 =
						path[z]->neighborsInSpanningTree[k]->getPosition().second;
				////convert the row to the original map row that was read from the image
				i1 = ((i1 + 0.5) * robotSizeInCells * 2);
				////convert the row to the original map column that was read from the image
				j1 = ((j1 + 0.5) * robotSizeInCells * 2);
				//check how we need to move on row or column
				//UP
				if (j < j1 && i == i1) {
					for (int m = j; m < j1; m++) {
						calcGridGraphindex(i, m);
						int c = (i * mapWidth + m) * 4;
						image[c] = 255;
						image[c + 1] = 0;
						image[c + 2] = 0;
					}
					//check how we need to move on row or column
					//DOWN
				} else if (j > j1 && i == i1) {
					for (int m = j1; m < j; m++) {
						int c = (i * mapWidth + m) * 4;
						image[c] = 255;
						image[c + 1] = 0;
						image[c + 2] = 0;
					}
					//check how we need to move on row or column
					//LEFT
				} else if (j == j1 && i > i1) {
					for (int m = i1; m < i; m++) {
						int c = (m * mapWidth + j) * 4;
						image[c] = 255;
						image[c + 1] = 0;
						image[c + 2] = 0;
					}

					//check how we need to move on row or column
					//RIGHT
				} else if (j == j1 && i < i1) {
					for (int m = i; m < i1; m++) {
						int c = (m * mapWidth + j) * 4;
						image[c] = 255;
						image[c + 1] = 0;
						image[c + 2] = 0;
					}
				}
				cout << "(" << path[z]->getPosition().first << ","
						<< path[z]->getPosition().second << ")";
				cout << " -> ";
				cout << "("
						<< path[z]->neighborsInSpanningTree[k]->getPosition().first
						<< ","
						<< path[z]->neighborsInSpanningTree[k]->getPosition().second
						<< ")" << endl;
			}
		}
	}
	cout << "save to image done" << endl;
	lodepng::encode(filePath, image, mapWidth, mapHeight);
}

void Map::addWayPoint(int x1, int y1, int x2, int y2, int r, int g, int b) {
	cout << "Way-point: " << "(" << x2 << "," << y2 << ")" <<endl;
	Node* node = new Node(x2, y2);
	wayPoints.push_back(node);
	drawline(x1, y1, x2, y2,  r,  g, b);
}

void Map::drawline(int x1, int y1, int x2, int y2, int r, int g, int b) {
	//check how we need to move on row or column

	//UP
	if (y1 < y2 && x1 == x2) {

		for (int m = y1; m < y2; m++) {
			int c = (x1 * mapWidth + m) * 4;
			image[c] = r;
			image[c + 1] = g;
			image[c + 2] = b;
		}
		//check how we need to move on row or column
		//DOWN
	} else if (y1 > y2 && x1 == x2) {
		for (int m = y2; m < y1; m++) {
			int c = (x1 * mapWidth + m) * 4;
			image[c] = r;
			image[c + 1] = g;
			image[c + 2] = b;
		}

		//check how we need to move on row or column
		//LEFT
	} else if (y1 == y2 && x1 > x2) {
		for (int m = x2; m < x1; m++) {
			int c = (m * mapWidth + y1) * 4;
			image[c] = r;
			image[c + 1] = g;
			image[c + 2] = b;
		}

		//check how we need to move on row or column
		//RIGHT
	} else if (y1 == y2 && x1 < x2) {
		for (int m = x1; m < x2; m++) {
			int c = (m * mapWidth + y1) * 4;
			image[c] = r;
			image[c + 1] = g;
			image[c + 2] = b;
		}
	}
};

void Map::addSTCPathToFile(char* filePath, vector<Node *> path) {


	int location = -1; // 0 = going left (blue), 1 = going right (black), 2 = down (magenta), 3 = up (green)

	for (unsigned int z = 0; z < path.size()-1; z++) {
		int this_x = path[z]->getPosition().first;
		int this_y = path[z]->getPosition().second;

		int next_x = path[z+1]->getPosition().first;
		int next_y = path[z+1]->getPosition().second;

		//convert the row to the original map row that was read from the image
		next_x = ((next_x + 0.5) * robotSizeInCells * 2);
		//convert the row to the original map column that was read from the image
		next_y = ((next_y + 0.5) * robotSizeInCells * 2);

		////convert the row to the original map row that was read from the image
		this_x = ((this_x + 0.5) * robotSizeInCells * 2);
		////convert the row to the original map column that was read from the image
		this_y = ((this_y + 0.5) * robotSizeInCells * 2);

		//buttom right
		int x_br = this_x + (0.5 * robotSizeInCells);
		//convert the row to the original map column that was read from the image
		int y_br = this_y + (0.5 * robotSizeInCells);
		int c = (x_br * (mapWidth) + y_br) * 4;
		image[c] = 0;
		image[c + 1] = 255;
		image[c + 2] = 0;

		//buttom left
		int x_bl = this_x + (0.5 * robotSizeInCells);
		//convert the row to the original map column that was read from the image
		int y_bl = this_y - (0.5 * robotSizeInCells);
		c = (x_bl * (mapWidth) + y_bl) * 4;
		image[c] = 255;
		image[c + 1] = 0;
		image[c + 2] = 0;

		//top right
		int x_tr = this_x - (0.5 * robotSizeInCells);
		//convert the row to the original map column that was read from the image
		int y_tr = this_y + (0.5 * robotSizeInCells);
		c = (x_tr * (mapWidth) + y_tr) * 4;
		image[c] = 0;
		image[c + 1] = 0;
		image[c + 2] = 0;

		//top left
		int x_tl = this_x - (0.5 * robotSizeInCells);
		//convert the row to the original map column that was read from the image
		int y_tl = this_y - (0.5 * robotSizeInCells);
		c = (x_tl * (mapWidth) + y_tl) * 4;
		image[c] = 255;
		image[c + 1] = 0;
		image[c + 2] = 255;

		//check how we need to move on row or column
		//This is for going from left to right on the map
		if (this_y < next_y && this_x == next_x) {

			//corner
			if (location == 2) {
				cout << "black c" << endl;
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),0,0,0);
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells), this_y + (0.5 * robotSizeInCells),0,0,0);
				//u turn
			} else if (location == 0) {
				cout << "black u" << endl;
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),0,0,0);
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),0,0,0);
				addWayPoint(this_x + (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells), this_y + (0.5 * robotSizeInCells),0,0,0);
			}

			cout << "black" << endl;
			drawline(this_x,this_y,next_x,next_y,255,0,0);
			addWayPoint(this_x + (0.5 * robotSizeInCells), this_y + (0.5 * robotSizeInCells),
					next_x + (0.5 * robotSizeInCells),next_y - (0.5 * robotSizeInCells),0,0,0);

			location = 1;

			//This is for going from right to left on the map
		} else if (this_y > next_y && this_x == next_x) {

			if (location == 3) {
				cout << "blue c" << endl;
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),0,0,255);
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),0,0,255);
			} else if (location == 1) {
				cout << "blue u" << endl;
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),0,0,255);
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),0,0,255);
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),0,0,255);
			}

			cout << "blue" << endl;
			drawline(this_x,this_y,next_x,next_y,255,0,0);
			addWayPoint(this_x - (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),
					next_x - (0.5 * robotSizeInCells),next_y + (0.5 * robotSizeInCells),0,0,255);
			location = 0;

			//This is for going from down to up on the map
		} else if (this_y == next_y && this_x > next_x) {

			if (location == 1) {
				cout << "green c" << endl;
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),0,255,0);
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells), this_y + (0.5 * robotSizeInCells),0,255,0);
			} else if (location == 2) {
				cout << "green u" << endl;
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),0,255,0);
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),0,255,0);
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells), this_y + (0.5 * robotSizeInCells),0,255,0);
			}

			cout << "green" << endl;
			drawline(this_x,this_y,next_x,next_y,255,0,0);
			addWayPoint(this_x - (0.5 * robotSizeInCells), this_y + (0.5 * robotSizeInCells),
					next_x + (0.5 * robotSizeInCells),next_y + (0.5 * robotSizeInCells),0,255,0);

			location = 3;

			//This is for going from up to down on the map
		} else if (this_y == next_y && this_x < next_x) {

			if (location == 0) {
				cout << "purple c" << endl;
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),255,0,255);
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),255,0,255);
			} else if (location == 3) {
				cout << "purple u" << endl;
				addWayPoint(this_x + (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),255,0,255);
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y + (0.5 * robotSizeInCells),
						this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),255,0,255);
				addWayPoint(this_x - (0.5 * robotSizeInCells),this_y - (0.5 * robotSizeInCells),
						this_x + (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),255,0,255);
			}

			cout << "purple" << endl;
			drawline(this_x,this_y,next_x,next_y,255,0,0);
			addWayPoint(this_x + (0.5 * robotSizeInCells), this_y - (0.5 * robotSizeInCells),
					next_x - (0.5 * robotSizeInCells),next_y - (0.5 * robotSizeInCells),255,0,255);

			location = 2;

		}

	}
	cout << "save to image done" << endl;
	lodepng::encode(filePath, image, mapWidth, mapHeight);
}



/**
 * calculate grid index from coarse grid
 */
void Map::calcGridGraphindex(int i, int j) {
	this->a = (((i - robotSizeInCells) / robotSizeInCells));
	this->b = (((j - robotSizeInCells) / robotSizeInCells));
}



void Map::createEdgeBetweenPoints(Node* point1, Node* point2) {
	int i = point1->getPosition().first;
	int j = point1->getPosition().second;
	//neighbor
	int i1 = point2->getPosition().first;
	int j1 = point2->getPosition().second;
	//convert the points to the real map
	i = ((i + 0.5) * robotSizeInCells);
	j = ((j + 0.5) * robotSizeInCells);
	i1 = ((i1 + 0.5) * robotSizeInCells);
	j1 = ((j1 + 0.5) * robotSizeInCells);

	//RIGHT
	if (i == i1 && j < j1) {
		for (int m = j; m < j1; m++) {
			int c = (i * mapWidth + m) * 4;
			image[c] = 255;
			image[c + 1] = 255;
			image[c + 2] = 0;
		}
	} else if (i == i1 && j > j1) {
		for (int m = j1; m < j; m++) {
			int c = (i * mapWidth + m) * 4;
			image[c] = 255;
			image[c + 1] = 255;
			image[c + 2] = 0;
		}
//LEFT
	} else if (i > i1 && j == j1) {
		for (int m = i1; m < i; m++) {
			int c = (m * mapWidth + j) * 4;
			image[c] = 255;
			image[c + 1] = 255;
			image[c + 2] = 0;
		}
	} else if (i < i1 && j == j1) {
		for (int m = i; m < i1; m++) {
			int c = (m * mapWidth + j) * 4;
			image[c] = 255;
			image[c + 1] = 255;
			image[c + 2] = 0;
		}
	}

	lodepng::encode("roboticLabMapnew2.png", image, mapWidth, mapHeight);
}

Node* Map::findNode(int x, int y) {
	for (unsigned int i = 0; i < this->forwardPath.size(); i++) {
		if ((this->forwardPath[i]->row == x)
				&& (this->forwardPath[i]->col == y))
			return this->forwardPath[i];
	}
	return NULL;
}

bool Map::checkIfRedline(Node* point1, Node* point2) {
	int i = point1->getPosition().first;
	int j = point1->getPosition().second;
	//neighbor
	int i1 = point2->getPosition().first;
	int j1 = point2->getPosition().second;
	//convert the points to the real map
	i = ((i + 0.5) * robotSizeInCells);
	j = ((j + 0.5) * robotSizeInCells);
	i1 = ((i1 + 0.5) * robotSizeInCells);
	j1 = ((j1 + 0.5) * robotSizeInCells);

	//RIGHT
	if (i == i1 && j < j1) {
		for (int m = j; m < j1; m++) {
			int c = (i * mapWidth + m) * 4;
			if ((image[c] == 255) && (image[c + 1] == 0) && (image[c + 2] == 0))
				return true;
		}
	} else if (i == i1 && j > j1) {
		for (int m = j1; m < j; m++) {
			int c = (i * mapWidth + m) * 4;
			if ((image[c] == 255) && (image[c + 1] == 0) && (image[c + 2] == 0))
				return true;
		}
//LEFT
	} else if (i > i1 && j == j1) {
		for (int m = i1; m < i; m++) {
			int c = (m * mapWidth + j) * 4;
			if ((image[c] == 255) && (image[c + 1] == 0) && (image[c + 2] == 0))
				return true;
		}
	} else if (i < i1 && j == j1) {
		for (int m = i; m < i1; m++) {
			int c = (m * mapWidth + j) * 4;
			if ((image[c] == 255) && (image[c + 1] == 0) && (image[c + 2] == 0))
				return true;
		}
	}
	return false;
}

void Map::drawPointsInfile(char* filePath, vector<vector<Node *> > graph) {
	cout << "this is grid map" << endl;
	for (unsigned int z = 0; z < fineMapHeight; z++) {
		for (unsigned int l = 0; l < fineMapWidth; l++) {
			if (graph[z][l] != NULL) {
				int i = graph[z][l]->getPosition().first;
				int j = graph[z][l]->getPosition().second;
				i = ((i + 0.5) * robotSizeInCells);
				//convert the row to the original map column that was read from the image
				j = ((j + 0.5) * robotSizeInCells);
				int c = (i * (mapWidth) + j) * 4;
				image[c] = 0;
				image[c + 1] = 0;
				image[c + 2] = 255;
			}

		}
	}
	lodepng::encode(filePath, image, mapWidth, mapHeight);
}
const Grid& Map::getFineGrid() const {
	return this->fineGrid;
}

const Grid& Map::getCoarseGrid() const {
	return this->coarseGrid;
}

Map::~Map() {
	// TODO Auto-generated destructor stub
}

