#ifndef GOTOWAYPOINT_H_
#define GOTOWAYPOINT_H_

#include "Behavior.h"
#include "../Robot/Robot.h"

// Responsible for bringing the robot to the his next way point, inherit Behavior
class GoToWayPoint: public Behavior {
public:
	GoToWayPoint(Robot* robot);
	bool StartCondition();
	bool StopCondition();
	void Action();
	virtual ~GoToWayPoint();

};

#endif /* GOTOWAYPOINT_H_ */
