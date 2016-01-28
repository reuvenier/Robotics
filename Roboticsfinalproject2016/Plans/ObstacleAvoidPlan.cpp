/*
 * ObstacleAvoidPlan.cpp
 *
 *  Created on: Jun 21, 2015
 *      Author: user
 */

#include "ObstacleAvoidPlan.h"
#include "../Behaviors/GoToWayPoint.h"
#include "../Behaviors/AvoidObstacle.h"
#include "../Defines.h"

ObstacleAvoidPlan::ObstacleAvoidPlan(Robot* robot, WaypointsManager* wpm) : Plan(robot){

	//Creating Behaviors
	_behaviors = new Behavior*[BEHAVIORS_COUNT];
	_behaviors[0] = new AvoidObstacle(robot, wpm);
	_behaviors[1] = new GoToWayPoint(robot, wpm);


	//Connecting behaviors
	_behaviors[0]->addNextBehavior(_behaviors[1]);
	_behaviors[1]->addNextBehavior(_behaviors[0]);

	_start = _behaviors[1];
}

Behavior* ObstacleAvoidPlan::getStartPoint(){
	return _start;
}

ObstacleAvoidPlan::~ObstacleAvoidPlan() {

	for(int i = 0; i < BEHAVIORS_COUNT; i++)
		delete _behaviors[i];
}
