 #include "Map.h"


Map::Map(float map_resolution, float robot_size, const char *file_path)
    :map_resolution_(map_resolution), robot_size_(robot_size) {

	fine_map_width_ = 0;
	fine_map_height_ = 0;
	coarse_map_width_ = 0;
	coarse_map_height_ = 0;
	map_width_ = 0;
	map_height_ = 0;
	map_resolution_ = 0;
	robot_size_ = robot_size;

	robot_size_in_cells_ = robot_size / map_resolution;
	inflation_radius_ = 0.3 * robot_size_in_cells_;

	this->LoadMapFromFile(file_path);
	this->InflateObstacles();
	this->BuildFineGrid();
	this->BuildCoarseGrid();


}

void Map::LoadMapFromFile(const char* file_path) {

	lodepng::decode(grid_image_, map_width_, map_height_, file_path);
	fine_map_width_ = map_width_ / robot_size_in_cells_;
	fine_map_height_ = map_height_ / robot_size_in_cells_;
	//create grid graph for to storage the path
	grid_graph_.resize(fine_map_height_);
	for (unsigned int k = 0; k < fine_map_height_; k++) {
		grid_graph_[k].resize(fine_map_width_);
	}
	cout << "map size: " << map_width_ << "," << map_height_ << endl;
	cout << "fine map size: " << fine_map_width_ << "," << fine_map_height_ << endl;

	map_.resize(map_height_);
	for (unsigned int i = 0; i < map_height_; i++)
		map_[i].resize(map_width_);

	for (unsigned int i = 0; i < map_height_; i++) {
		for (unsigned int j = 0; j < map_width_; j++) {
			map_[i][j] = CheckIfCellIsOccupied(i, j);
		}
	}
}

bool Map::CheckIfCellIsOccupied(int i, int j) {

	int c = (i * map_width_ + j) * 4;
	int r = grid_image_[c];
	int g = grid_image_[c + 1];
	int b = grid_image_[c + 2];

	if (r == 0 && g == 0 && b == 0)
		return true;
	return false;
}

void Map::InflateObstacles() {

	inflate_map_.resize(map_height_);
	for (unsigned int i = 0; i < map_height_; i++) {
		inflate_map_[i].resize(map_width_);
	}

	for (unsigned int i = 0; i < map_height_; i++) {
		for (unsigned int j = 0; j < map_width_; j++) {
			if (map_[i][j]) {
				Inflate(i, j);
			}
		}
	}

	lodepng::encode("inflated_map.png",grid_image_,map_width_, map_height_);
}

void Map::Inflate(unsigned int i,unsigned int j) {

	for (unsigned int k = i - inflation_radius_; k < i + inflation_radius_; k++) {
		for (unsigned int m = j - inflation_radius_; m < j + inflation_radius_; m++) {
			if (m < 0 || k < 0 || k >= map_height_ || m >= map_width_) {
				continue;
			} else {
				// Mark this cell as occupied in inflateMap
				inflate_map_[k][m] = true;
				// Draw black color in corresponding image
				int c = (k * map_width_ + m) * 4;
				grid_image_[c] = 0;
				grid_image_[c + 1] = 0;
				grid_image_[c + 2] = 0;
			}
		}
	}
}

void Map::BuildFineGrid() {

	int fine_grid_height = map_height_ / robot_size_in_cells_;
	int fine_grid_width = map_width_ / robot_size_in_cells_;
	fine_grid_.resize(fine_grid_height);
	for (int i = 0; i < fine_grid_height; i++) {
		fine_grid_[i].resize(fine_grid_width);
	}

	int flag = false;

	for (int i = 0; i < fine_grid_height; i++) {
		for (int j = 0; j < fine_grid_width; j++) {
			flag = false;
			for (int m = i * robot_size_in_cells_;
					m < i * robot_size_in_cells_ + robot_size_in_cells_; m++) {
				for (int z = j * robot_size_in_cells_;
						z < j * robot_size_in_cells_ + robot_size_in_cells_; z++) {
					if (inflate_map_[m][z] == true) {
						flag = true;
						break;
					}
				}
			}
			fine_grid_[i][j] = flag;
		}
	}
}

void Map::BuildCoarseGrid() {

	coarse_map_height_ = map_height_ / (robot_size_in_cells_ * 2);
	coarse_map_width_ = map_width_ / (robot_size_in_cells_ * 2);
	coarse_grid_.resize(coarse_map_height_);

	for (unsigned int i = 0; i < coarse_map_height_; i++) {
		coarse_grid_[i].resize(coarse_map_width_);
	}

	int fleg = false;

	for (unsigned int i = 0; i < coarse_map_height_; i++) {
		for (unsigned int j = 0; j < coarse_map_width_; j++) {
			fleg = false;
			for (int m = i * (robot_size_in_cells_ * 2);
					m < i * (robot_size_in_cells_ * 2) + (robot_size_in_cells_ * 2);
					m++) {
				for (int z = j * (robot_size_in_cells_ * 2);
						z < j * (robot_size_in_cells_ * 2) + (robot_size_in_cells_ * 2);
						z++) {
					if (inflate_map_[m][z] == true) {
						fleg = true;
						break;
					}
				}
			}
			coarse_grid_[i][j] = fleg;
		}
	}
}

///**
// * add the full path from STC to the map (red line)
// */
//void Map::AddPathToFile(char* file_Path, vector<Node *> path) {
//
//
//	int location = -1; // 0 = going left (blue), 1 = going right (black), 2 = down (magenta), 3 = up (green)
//
//	for (unsigned int z = 0; z < path.size()-1; z++) {
//		int this_x = path[z]->GetPosition().first;
//		int this_y = path[z]->GetPosition().second;
//
//		int next_x = path[z+1]->GetPosition().first;
//		int next_y = path[z+1]->GetPosition().second;
//
//		//convert the row to the original map row that was read from the image
//		next_x = ((next_x + 0.5) * robot_size_in_cells_ * 2);
//		//convert the row to the original map column that was read from the image
//		next_y = ((next_y + 0.5) * robot_size_in_cells_ * 2);
//
//		////convert the row to the original map row that was read from the image
//		this_x = ((this_x + 0.5) * robot_size_in_cells_ * 2);
//		////convert the row to the original map column that was read from the image
//		this_y = ((this_y + 0.5) * robot_size_in_cells_ * 2);
//
//
//		//check how we need to move on row or column
//		//This is for going from left to right on the map
//		if (this_y < next_y && this_x == next_x) {
//
//			//corner
//			if (location == 2) {
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),0,0,0);
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_), this_y + (0.5 * robot_size_in_cells_),0,0,0);
//				//u turn
//			} else if (location == 0) {
//				cout << "black u" << endl;
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),0,0,0);
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),0,0,0);
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_), this_y + (0.5 * robot_size_in_cells_),0,0,0);
//			}
//
//
//			Drawline(this_x + (0.5 * robot_size_in_cells_), this_y + (0.5 * robot_size_in_cells_),
//					next_x + (0.5 * robot_size_in_cells_),next_y - (0.5 * robot_size_in_cells_),0,0,0);
//
//			location = 1;
//
//			//This is for going from right to left on the map
//		} else if (this_y > next_y && this_x == next_x) {
//
//			if (location == 3) {
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),0,0,255);
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),0,0,255);
//
//			} else if (location == 1) {
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),0,0,255);
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),0,0,255);
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),0,0,255);
//			}
//
//
//			Drawline(this_x - (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),
//					next_x - (0.5 * robot_size_in_cells_),next_y + (0.5 * robot_size_in_cells_),0,0,255);
//			location = 0;
//
//			//This is for going from down to up on the map
//		} else if (this_y == next_y && this_x > next_x) {
//
//			if (location == 1) {
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),0,255,0);
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_), this_y + (0.5 * robot_size_in_cells_),0,255,0);
//
//			} else if (location == 2) {
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),0,255,0);
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),0,255,0);
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_), this_y + (0.5 * robot_size_in_cells_),0,255,0);
//			}
//
//
//
//			Drawline(this_x - (0.5 * robot_size_in_cells_), this_y + (0.5 * robot_size_in_cells_),
//					next_x + (0.5 * robot_size_in_cells_),next_y + (0.5 * robot_size_in_cells_),0,255,0);
//
//			location = 3;
//
//			//This is for going from up to down on the map
//		} else if (this_y == next_y && this_x < next_x) {
//
//			if (location == 0) {
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),255,0,255);
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),255,0,255);
//
//			} else if (location == 3) {
//
//				Drawline(this_x + (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),255,0,255);
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y + (0.5 * robot_size_in_cells_),
//						this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),255,0,255);
//
//				Drawline(this_x - (0.5 * robot_size_in_cells_),this_y - (0.5 * robot_size_in_cells_),
//						this_x + (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),255,0,255);
//			}
//
//
//
//			Drawline(this_x + (0.5 * robot_size_in_cells_), this_y - (0.5 * robot_size_in_cells_),
//					next_x - (0.5 * robot_size_in_cells_),next_y - (0.5 * robot_size_in_cells_),255,0,255);
//
//			location = 2;
//
//		}
//
//	}
//
//	lodepng::encode(file_Path,grid_image_,map_width_, map_height_);
//}
//
//
//void Map::Drawline(int x1, int y1, int x2, int y2, int r, int g, int b) {
//	//check how we need to move on row or column
//
//	//UP
//	if (y1 < y2 && x1 == x2) {
//
//		for (int m = y1; m < y2; m++) {
//			int c = (x1 * map_width_ + m) * 4;
//			grid_image_[c] = r;
//			grid_image_[c + 1] = g;
//			grid_image_[c + 2] = b;
//		}
//		//check how we need to move on row or column
//		//DOWN
//	} else if (y1 > y2 && x1 == x2) {
//		for (int m = y2; m < y1; m++) {
//			int c = (x1 * map_width_ + m) * 4;
//			grid_image_[c] = r;
//			grid_image_[c + 1] = g;
//			grid_image_[c + 2] = b;
//		}
//
//		//check how we need to move on row or column
//		//LEFT
//	} else if (y1 == y2 && x1 > x2) {
//		for (int m = x2; m < x1; m++) {
//			int c = (m * map_width_ + y1) * 4;
//			grid_image_[c] = r;
//			grid_image_[c + 1] = g;
//			grid_image_[c + 2] = b;
//		}
//
//		//check how we need to move on row or column
//		//RIGHT
//	} else if (y1 == y2 && x1 < x2) {
//		for (int m = x1; m < x2; m++) {
//			int c = (m * map_width_ + y1) * 4;
//			grid_image_[c] = r;
//			grid_image_[c + 1] = g;
//			grid_image_[c + 2] = b;
//		}
//	}
//}

void Map::AddPathToFile(const char* file_path , vector<vector<Node *> > graph, int width, int height) {

	cout<<"file path " <<file_path<<endl;

	cout<<"Graph length " << graph.size()<<endl;


	for(int z = 0;z<height;z++){
		for(int l= 0;l<width;l++){
			if (graph[z][l] != NULL) {
				for (int k=0;k<4;k++) {
					if (graph[z][l]->neighbors_in_spanning_tree_[k] != NULL) {
						int i = graph[z][l]->GetPosition().first;
						int j = graph[z][l]->GetPosition().second;
						//convert the row to the original map row that was read from the image
						i  = ((i+0.5) * robot_size_in_cells_ * 2);
						//convert the row to the original map column that was read from the image
						j = ((j+0.5) * robot_size_in_cells_ * 2);

						int i1 = graph[z][l]->neighbors_in_spanning_tree_[k]->GetPosition().first ;
						int j1 = graph[z][l]->neighbors_in_spanning_tree_[k]->GetPosition().second;
						////convert the row to the original map row that was read from the image
						i1 = ((i1+0.5) * robot_size_in_cells_ * 2);
						////convert the row to the original map column that was read from the image
						j1 = ((j1+0.5) * robot_size_in_cells_ * 2);
						//check how we need to move on row or column
						if(j<j1 && i == i1){
							for (int m = j; m < j1; m++) {
								int c = (i * map_width_ + m) * 4;
								grid_image_[c] = 255;
								grid_image_[c + 1] = 0;
								grid_image_[c + 2] = 0;

							}
							//check how we need to move on row or column
						}else if(j>j1 && i == i1){
							for (int m =j1 ; m < j; m++) {
								int c = (i * map_width_ + m) * 4;
								grid_image_[c] = 255;
								grid_image_[c + 1] = 0;
								grid_image_[c + 2] = 0;
							}
							//check how we need to move on row or column
						}else if(j == j1 && i > i1){
							for (int m =i1 ; m < i; m++) {
								int c = (m * map_width_ + j) * 4;
								grid_image_[c] = 255;
								grid_image_[c + 1] = 0;
								grid_image_[c + 2] = 0;
							}
							//check how we need to move on row or column
						}else if(j == j1 && i < i1){
							for (int m =i ; m < i1; m++) {
								int c = (m * map_width_ + j) * 4;
								grid_image_[c] = 255;
								grid_image_[c + 1] = 0;
								grid_image_[c + 2] = 0;
							}
						}
						cout << "(" << graph[z][l]->GetPosition().first << "," << graph[z][l]->GetPosition().second << ")";
						cout << " -> ";
						cout << "(" << graph[z][l]->neighbors_in_spanning_tree_[k]->GetPosition().first << "," << graph[z][l]->neighbors_in_spanning_tree_[k]->GetPosition().second << ")" << endl;
					}
				}

			}

		}
	}
	unsigned error = lodepng::encode(file_path, grid_image_, map_width_, map_height_);

	if (!error){
		std::cout << "encoder error " << error << ": "
		<< lodepng_error_text(error) << std::endl;
	}
}

const Grid& Map::GetFineGrid() const {
	return this->fine_grid_;
}

const Grid& Map::GetCoarseGrid() const {
	return this->coarse_grid_;
}

const Grid& Map::GetInflateMap() const {
	return inflate_map_;
}

Map::~Map() {

}

