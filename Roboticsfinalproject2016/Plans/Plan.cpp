/*
 * Plan.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: user
 */

#include "Plan.h"
#include "../Behaviors/Behavior.h"

Plan::Plan(Robot* robot) {
	 _robot = robot;
	 _behaviors = NULL;
	 _start = NULL;
}

Plan::~Plan() {

}

Behavior* Plan::getStartPoint(){
	return _start;
}
