#ifndef OBSTACLEAVOIDPLAN_H_
#define OBSTACLEAVOIDPLAN_H_

#include "Plan.h"

// Inherit plan, implement avoid obstacle plan,
// using GoToWayPoint behavior and AvoidObstacle behavior
class ObstacleAvoidPlan: public Plan {

public:

	ObstacleAvoidPlan(Robot* robot);
	Behavior* GetStartPoint();
	virtual ~ObstacleAvoidPlan();
};

#endif /* OBSTACLEAVOIDPLAN_H_ */


