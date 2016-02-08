#include <iostream>
#include "Managers/ConfigurationManager.h"
#include "Managers/Manager.h"
#include "Map/Map.h"
#include "Plans/ObstacleAvoidPlan.h"
#include "Robot/Robot.h"
#include "Robot/Robot.h"
#include "STC/Node.h"
#include "STC/Stc.h"
#include <vector>

using namespace std;

const char* kConfigurationPath = "Configuration";
const char* kUrlToPlayer = "localhost";

const char* kInflatePathWithStc = "infalted_map_with_stc_path.png";
const int kPortToPlayer = 6665;

int main()
{

	ConfigurationManager* Configuration_manager = new ConfigurationManager(kConfigurationPath);

	Map map(Configuration_manager->getMapResolution(), Configuration_manager->getRobotWidth(),
			Configuration_manager->getMapPath());

	Robot robot(kUrlToPlayer, kPortToPlayer, Configuration_manager);

	Position startPos;
	startPos.first = 6;
	startPos.second = 17;

	Stc stc(map, startPos);

	cout<<"Adding STC path to the Map"<<endl;


    ObstacleAvoidPlan plan(&robot);

    vector <Node *>  path = stc.GetWayPointsPath();
    cout<<"STC nodes"<<endl;
    for (unsigned int i=0; i<path.size(); ++i){
    	cout<<path[i]->GetPosition().first<< " ," <<path[i]->GetPosition().second<<endl;
    }

    map.AddPathToFile(kInflatePathWithStc, stc.GetGraph(), stc.getGraphWidth(), stc.getGraphHeight());

    robot.SetWayPoints(path);

    Manager manager(&robot, &plan);

    manager.run();
    delete Configuration_manager;

 	return 0;
}


