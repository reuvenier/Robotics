/*
 * main.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: user
 */
#include "STC/Node.h"
#include <iostream>
#include "Robot/Robot.h"
#include "Managers/Manager.h"
#include "Plans/ObstacleAvoidPlan.h"
#include "Map/Map.h"
#include "Managers/ConfigurationManager.h"
#include <vector>
#include "Managers/WaypointsManager.h"
#include "STC/STC.h"
#include "Robot/Robot.h"
using namespace std;

int main()
{

    cout<<"Load config init Map and Robot"<<endl;
	//load the file data into the ConfigurationManager data members
	ConfigurationManager ConfigMgr(CONFIGURATION_PATH);


	Map map(ConfigMgr.map_resolution, ConfigMgr.robot_length);

	Robot robot("localhost", 6665, &ConfigMgr, map.Gridimage.size());

	cout<<"Building Map, loading, inflate, build coarse and fine grid"<<endl;
	map.loadMapFromFile(ConfigMgr.map_path);
	map.inflateObstacles();
	map.buildFineGrid();
	map.buildCoarseGrid();
	Position startPos;

	//TODO initialized the startPos with data from ConfigMgr
	startPos.first = 6;
	startPos.second = 17;

	cout<< "First Position: " << startPos.first<< " ,Second Position: " << startPos.second<< endl;

	cout<<"STC starting" <<endl;
	STC stc(map, startPos);
	vector <Node *>  path = stc.getPath();
    //Building the way points vector.
	WaypointsManager wpm(path, ConfigMgr.grid_resolution, ConfigMgr.map_resolution);

	cout<<"Adding STC path to the Map"<<endl;

	map.addDFSPathToFile("NewMap.png", path);
    vector<Node*> wayPoints = wpm.getPathInNodes();
	map.addSTCPathToFile("roboticLabNew.png",wayPoints);

	cout<<"Printing the nodes on the path "<<endl;
    for (int i = 0; i < wayPoints.size(); ++i) {
		cout<<"(" << wayPoints[i]->col<<", "<<wayPoints[i]->row<< ")"<<endl;
	}

    cout<<"Obstacle avoid plan"<<endl;
    ObstacleAvoidPlan pln(&robot, &wpm);
    LocalizationManager lm;

    cout<<"starting manager"<<endl;
    Manager manager(&robot, &pln, &lm, &ConfigMgr, &wpm);
    manager.run();

 	return 0;
}


