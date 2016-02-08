#include "ObstacleAvoidPlan.h"
#include "../Behaviors/GoToWayPoint.h"
#include "../Behaviors/AvoidObstacle.h"
#include "../Behaviors/Behavior.h"


ObstacleAvoidPlan::ObstacleAvoidPlan(Robot* robot) : Plan(robot){

	behaviors_ = new Behavior*[kBhehaviorsAmmount];
	behaviors_[0] = new AvoidObstacle(robot);
	behaviors_[1] = new GoToWayPoint(robot);

	//Connecting behaviors
	behaviors_[0]->AddNextBehavior(behaviors_[1]);
	behaviors_[1]->AddNextBehavior(behaviors_[0]);
	start_ = behaviors_[1];
}

Behavior* ObstacleAvoidPlan::GetStartPoint(){
	return start_;
}

ObstacleAvoidPlan::~ObstacleAvoidPlan() {

	for(int i = 0; i < kBhehaviorsAmmount; i++)
		delete behaviors_[i];
}
