#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>

#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

using namespace std;

#define NUM_OF_PROPERTIES 10
#define MAX_LINE_LENGTH 100



// Responsible of loading the initial values.
class ConfigurationManager {

public:

	static const double kMapWidth = 550;
	static const double kMapLength = 380;
	static const double kPlayerMapWidth = 13.75;
	static const double kPlayerMapLength = 9.5;

private:
	char file_data_[NUM_OF_PROPERTIES][MAX_LINE_LENGTH];
	const char * map_path_;
	int start_x_;
	int start_y_;
	int start_yaw_;
	double robot_length_;
	double robot_width_;
	double map_resolution_;
	double grid_resolution_;
	char* configurationPath_;

	// Gets the configuration file path and load the data
	// to the relevant data members
	void ReadConfigFile(const char* configuration_path);

public:
	// C'tor, get the configuration path from the out side and
	// calls readConfigFile() with the path
	ConfigurationManager(const char* configuration_path);


	char* getConfigurationPath() const {
		return configurationPath_;
	}

	double getGridResolution() const {
		return grid_resolution_;
	}

	const char* getMapPath() const {
		return map_path_;
	}

	double getMapResolution() const {
		return map_resolution_;
	}

	double getRobotLength() const {
		return robot_length_;
	}

	double getRobotWidth() const {
		return robot_width_;
	}

	int getStartX() const {
		return start_x_;
	}

	int getStartY() const {
		return start_y_;
	}

	int getStartYaw() const {
		return start_yaw_;
	}

	virtual ~ConfigurationManager();
};

#endif /* CONFIGURATIONMANAGER_H_ */
