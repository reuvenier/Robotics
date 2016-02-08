#include "ConfigurationManager.h"

using namespace std;

ConfigurationManager::ConfigurationManager(const char* configuration_path){
	ReadConfigFile(configuration_path);
}

void ConfigurationManager::ReadConfigFile(const char* configPath)
{
			string map_path;
			string temp;
			double map_resolution;
			double grid_resolution;
			double start_location_x;
			double start_location_y;
			double start_location_yaw;
			double robot_size_x;
			double robot_size_y;



			ifstream fin(configPath);
			string line;
			istringstream sin;

			// Getting the map path
			getline(fin, line);
			istringstream aa(line);
			aa >> temp >> map_path;

			this->map_path_ = (char*)map_path.c_str();

			// Getting the start location
			getline(fin, line);
			istringstream bb(line);
			bb >> temp >> start_location_x >> start_location_y >> start_location_yaw;
			this-> start_x_ = start_location_x;
			this-> start_y_ = start_location_y;
			this-> start_yaw_ = start_location_yaw;

			// Getting the robot size
			getline(fin, line);
			istringstream dd(line);
			dd >> temp >> robot_size_x >> robot_size_y;
			this-> robot_width_ = robot_size_x;
			this-> robot_length_ = robot_size_y;

			// Getting the map resolution
			getline(fin, line);
			istringstream ee(line);
			ee >> temp >> map_resolution;
			this-> map_resolution_ = map_resolution;

			// Getting the grid resolution
			getline(fin, line);
			istringstream ff(line);
			ff >> temp >> grid_resolution;
			this-> grid_resolution_ = grid_resolution;

			fin.close();

			cout<< "hello"<< map_resolution<<endl;
}

ConfigurationManager::~ConfigurationManager() {

}
