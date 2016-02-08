#include "Plan.h"
#include "../Behaviors/Behavior.h"

Plan::Plan(Robot* robot) {
	 robot_ = robot;
	 behaviors_ = NULL;
	 start_ = NULL;
}

Plan::~Plan() {

}

