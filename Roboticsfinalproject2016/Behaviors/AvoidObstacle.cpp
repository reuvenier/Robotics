/*
 * AvoidObstacle.cpp
 *
 *  Created on: Jan 28, 2016
 *      Author: colman
 */

#include "AvoidObstacle.h"

AvoidObstacle::AvoidObstacle(Robot* robot,  WaypointsManager* Wpm):Behavior(robot) {
	_wpm = Wpm;
}

bool AvoidObstacle::startCond(){
	return !_robot->isForwardFree();
}

bool AvoidObstacle::stopCond(){
	return _robot->isForwardFree();
	}

void AvoidObstacle::action(){
	_robot->setSpeed(0.0,0.2);
	}

AvoidObstacle::~AvoidObstacle() {

}

