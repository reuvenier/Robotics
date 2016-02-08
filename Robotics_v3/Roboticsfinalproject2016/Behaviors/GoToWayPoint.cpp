#include "GoToWayPoint.h"

GoToWayPoint::GoToWayPoint(Robot* robot):Behavior(robot) {

}

bool GoToWayPoint::StartCondition(){

	return robot_->FreeInFront(robot_->kDistanceTolerance);
}

bool GoToWayPoint::StopCondition()
{
	return (robot_->CheckIfReachToDestination() || !robot_->FreeInFront(robot_->kDistanceTolerance ));
}
void GoToWayPoint::Action()
{
	robot_->GoToNextWaypoint();
}

GoToWayPoint::~GoToWayPoint(){

}
