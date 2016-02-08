
#include "AvoidObstacle.h"

AvoidObstacle::AvoidObstacle(Robot* robot):Behavior(robot) {

}

bool AvoidObstacle::StartCondition(){
	return !robot_->FreeInFront(robot_->kDistanceTolerance);
}

bool AvoidObstacle::StopCondition(){
	return robot_->FreeInFront(robot_->kDistanceTolerance);
}

void AvoidObstacle::Action(){
	robot_->SetSpeed(0.0,0.2);
}

AvoidObstacle::~AvoidObstacle() {

}

