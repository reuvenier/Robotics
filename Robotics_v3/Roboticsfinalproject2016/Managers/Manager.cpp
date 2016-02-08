#include "Manager.h"

Manager::Manager(Robot* robot, Plan* plan) {
	robot_ = robot;
	behavior_ = plan->GetStartPoint();
}

void Manager::run()
{
	robot_->Read();
	while(behavior_){
		robot_->Read();
		while(!(behavior_->StopCondition())){
			behavior_->Action();
			robot_->Read();
		}
		behavior_ = behavior_->GetNext();
	}
}

Manager::~Manager() {

}
