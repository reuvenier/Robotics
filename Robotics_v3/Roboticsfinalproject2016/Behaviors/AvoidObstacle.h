#ifndef AVOIDOBSTACLE_H_
#define AVOIDOBSTACLE_H_

#include "Behavior.h"

const float kAvoidenceYaw = 0.2;

// Responsible for avoiding obstacles, inherit Behavior
class AvoidObstacle: public Behavior {

public:

	AvoidObstacle(Robot* robot);
	bool StartCondition();
	bool StopCondition();
	void Action();

	virtual ~AvoidObstacle();

private:


};

#endif /* AVOIDOBSTACLE_H_ */
