/*
 * AvoidObstacle.h
 *
 *  Created on: Jan 28, 2016
 *      Author: colman
 */

#ifndef AVOIDOBSTACLE_H_
#define AVOIDOBSTACLE_H_

#include "../Managers/WaypointsManager.h"
#include "Behavior.h"

class AvoidObstacle: public Behavior {

public:

	AvoidObstacle(Robot* robot,  WaypointsManager* Wpm);
	bool startCond();
	bool stopCond();
	void action();


	virtual ~AvoidObstacle();

private:
	WaypointsManager* _wpm;

};

#endif /* AVOIDOBSTACLE_H_ */
