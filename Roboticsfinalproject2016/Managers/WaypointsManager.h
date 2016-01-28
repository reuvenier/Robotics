/*
 * WaypointsManager.h
 *
 *  Created on: Jun 14, 2015
 *      Author: colman
 */

#ifndef WAYPOINTSMANAGER_H_
#define WAYPOINTSMANAGER_H_

#include "../WayPoint/wayPoint.h"
#include "../STC/Node.h"
#include "../Defines.h"
#include <iostream>
#include <vector>
#include <math.h>
#include<set>

using std::vector;
using std::set;

class WaypointsManager {

public:

	//Data Members
	vector<vector<Node *> > gridGraph;
	vector<Node *>  forwardPath;
	set<wayPoint> wayPoints;
    unsigned int wayPointArrayIndex;
    wayPoint currentWayPoint, nextWayPoint;

    // Methods
    WaypointsManager(vector<Node*> path, double gridResolution, double mapResolution);
    void build_way_point_vector(vector<Node *> path);
    bool isInWayPoint(double x,double y);
    void convertPathToWayPoints();
	void setNextWayPoint(wayPoint Next);
	wayPoint getCurrentWayPoint();
	wayPoint getNextWayPoint();
	vector<Node*> getPathInNodes();
	set<wayPoint> getPathInWayPoints();
	virtual ~WaypointsManager();

private:
    //Private data members
	double _gridResolution;
	double _mapResolution;



};

#endif /* WAYPOINTSMANAGER_H_ */
