#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "../Robot/Robot.h"


// Pure virtual class class, responsible for holding the next
// behaviors. describes the robot behavior e.g go forward.
// startCondition, stopCondition and action must be implemented.
class Behavior {

protected:
	Robot* robot_;
	Behavior** behaviors_array_;
	int size_of_next_behavior_array_;

public:
	//C'tor
	Behavior(Robot* robot);

	//returns a boolean.
	// True if the robot can start the action of this behavior.
	virtual bool StartCondition()=0;

	//returns a boolean.
	// True if the robot can NOT start the action of this behavior.
	virtual bool StopCondition()=0;

	//Perform the action of the inherited behavior.
	virtual void Action() = 0;

	// Returns a boolean if succeed.
	// Allocate a new behavior in behavior array, and
	// insert it. It is the client responsibility to delete the behaviors.
	// This class will delete the behaviors array.
	bool AddNextBehavior(Behavior* behavior);

	// Returns a behavior, the next available.
	Behavior* GetNext();

	//D'tor
	virtual ~Behavior();
};

#endif /* BEHAVIOR_H_ */
