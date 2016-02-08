#ifndef MANAGER_H_
#define MANAGER_H_

#include "../Plans/ObstacleAvoidPlan.h"
#include "../Robot/Robot.h"

//Managing the robot plan.
class Manager {
	Behavior* behavior_;
	Robot* robot_;


public:
	//C'tor
	Manager(Robot* robot, Plan* plan);

	// Getting the plan start behavior and iterates to
	// next behavior according to the plan.
	void run();

	//D'tor
	virtual ~Manager();
};

#endif /* MANAGER_H_ */
