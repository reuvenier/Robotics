#include "Behavior.h"

Behavior::Behavior(Robot* robot) {
	robot_ = robot;
	size_of_next_behavior_array_ = 0;
	behaviors_array_ = NULL;
}
bool Behavior::AddNextBehavior(Behavior* behavior)
{
	Behavior** save = behaviors_array_;
	int i;
	behaviors_array_ = new Behavior*[size_of_next_behavior_array_+1];
	if(!behaviors_array_){
		behaviors_array_ = save;
		return false;
	}
	size_of_next_behavior_array_++;
	for(i=0;i<size_of_next_behavior_array_-1;i++){
		behaviors_array_[i] = save[i];
	}
	behaviors_array_[i] = behavior;
	delete []save;
	return true;
}
Behavior* Behavior:: GetNext()
{
	int i;
	for(i=0;i<size_of_next_behavior_array_;i++){
		if(behaviors_array_[i]->StartCondition()){
			return behaviors_array_[i];
		}
	}

	return NULL;
}
Behavior::~Behavior() {
	delete [] behaviors_array_;
}
