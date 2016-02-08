#ifndef PLAN_H_
#define PLAN_H_

#include "../Robot/Robot.h"
#include "../Behaviors/Behavior.h"

const int kBhehaviorsAmmount = 2;

// Pure virtual class, Holding an array of behaviors,
// the plan should implement the logic in the constructor.
// The client is responsible to delete the added behaviors.
class Plan {

protected:
	Robot* robot_;
	Behavior* start_;
	Behavior** behaviors_;

public:
	Plan(Robot* robot);

	// Returns the start behavior of the plan.
	virtual Behavior* GetStartPoint()=0;
	virtual ~Plan();
};

#endif /* PLAN_H_ */
