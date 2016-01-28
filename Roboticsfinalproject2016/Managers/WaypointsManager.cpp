
#include "WaypointsManager.h"
#include <math.h>
using namespace std;

WaypointsManager::WaypointsManager(vector<Node*> path, double gridResolution, double mapResolution){

	this->wayPointArrayIndex = 0;
	this->_gridResolution = gridResolution;
	this->_mapResolution = mapResolution;

	cout<<"Building the way points"<<endl;

	build_way_point_vector(path);
	convertPathToWayPoints();

}



void WaypointsManager::build_way_point_vector(vector<Node *> path)
{
	for (unsigned int z = 0; z < path.size(); z++) {
			if (path[z] != NULL) {
				for (int k = 0; k < 4; k++) {
					if (path[z]->neighborsInSpanningTree[k] != NULL) {
						int i = path[z]->getPosition().first;
						int j = path[z]->getPosition().second;
						//convert the row to the original map row that was read from the image
						i = (i * 2) + 0.5;
						//convert the row to the original map column that was read from the image
						j = (j * 2) + 0.5;
						gridGraph[i][j] = new Node(i, j);
						gridGraph[i][j]->setIfExistWall(true);
						gridGraph[i][j + 1] = new Node(i, j + 1);
						gridGraph[i][j + 1]->setIfExistWall(true);
						//createEdgeBetweenPoints(gridGraph[i][j],gridGraph[i][j + 1]);
						gridGraph[i + 1][j] = new Node(i + 1, j);
						gridGraph[i + 1][j]->setIfExistWall(true);
						gridGraph[i + 1][j + 1] = new Node(i + 1, j + 1);
						gridGraph[i + 1][j + 1]->setIfExistWall(true);
						forwardPath.push_back(new Node(i, j));
						forwardPath.push_back(new Node(i, j + 1));
						forwardPath.push_back(new Node(i + 1, j));
						forwardPath.push_back(new Node(i + 1, j + 1));

						//createEdgeBetweenPoints(gridGraph[i + 1][j],gridGraph[i + 1][j + 1]);
						int i1 =
								path[z]->neighborsInSpanningTree[k]->getPosition().first;
						int j1 =
								path[z]->neighborsInSpanningTree[k]->getPosition().second;
						////convert the coarse row to the grid map row
						i1 = (i1 * 2) + 0.5;
						////convert the coarse row to the grid map row
						j1 = (j1 * 2) + 0.5;
						gridGraph[i1][j1] = new Node(i1, j1);
						gridGraph[i1][j1]->setIfExistWall(true);
						gridGraph[i1][j1 + 1] = new Node(i1, j1 + 1);
						gridGraph[i1][j1 + 1]->setIfExistWall(true);
						//createEdgeBetweenPoints(gridGraph[i1][j1],gridGraph[i1][j1 + 1]);
						gridGraph[i1 + 1][j1] = new Node(i1 + 1, j1);
						gridGraph[i1 + 1][j1]->setIfExistWall(true);
						gridGraph[i1 + 1][j1 + 1] = new Node(i1 + 1, j1 + 1);
						gridGraph[i1 + 1][j1 + 1]->setIfExistWall(true);
						//createEdgeBetweenPoints(gridGraph[i1 + 1][j1],gridGraph[i1 + 1][j1 + 1]);
						forwardPath.push_back(new Node(i1, j1));
						forwardPath.push_back(new Node(i1, j1 + 1));
						forwardPath.push_back(new Node(i1 + 1, j1));
						forwardPath.push_back(new Node(i1 + 1, j1 + 1));

					}
				}

			}

		}
}
void WaypointsManager::convertPathToWayPoints(){

	set<wayPoint>::iterator iter = wayPoints.begin();

	for (unsigned int i = 0; i < forwardPath.size(); ++i) {
		Position p = forwardPath[i]->getPosition();
		double x = (double)p.first;
		double y = (double)p.second;
		wayPoint wp(x, y);
		wayPoints.insert(iter, wp);
		++iter;

	}
}


bool WaypointsManager::isInWayPoint(double x,double y)
{
	double dx = nextWayPoint.x - x;
	double dy = nextWayPoint.y - y;
	double distance = sqrt(pow(dx, 2) + pow(dy, 2));

	cout << "Next way point x: "<< nextWayPoint.x << " ---> current x: " << x << endl;
	cout << "Next way point y: "<< nextWayPoint.y << " ---> current y: " << y << endl;
	cout << "Distance to next way point: " << (distance) << endl;
	cout << endl;

	// Checking if the robot hit the way point, considering tolerance
	if (distance*_gridResolution <= TOLERANCE)
	{
		return true;
	}
	return false;
}

void WaypointsManager::setNextWayPoint(wayPoint Next)
{
	currentWayPoint.x = nextWayPoint.x;
	currentWayPoint.y = nextWayPoint.y;


	nextWayPoint.x = Next.x;
	nextWayPoint.y = Next.y;

}

wayPoint WaypointsManager::getNextWayPoint(){

	return this->nextWayPoint;

}

wayPoint WaypointsManager::getCurrentWayPoint(){

	return this->currentWayPoint;
}

vector<Node*> WaypointsManager::getPathInNodes(){
    return this->forwardPath;
}

set<wayPoint> WaypointsManager::getPathInWayPoints(){

	return this->wayPoints;
	}



WaypointsManager::~WaypointsManager() {

}
