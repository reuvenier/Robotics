
#include "Manager.h"

Manager::Manager(Robot* robot, Plan* pln, LocalizationManager* l_manager, ConfigurationManager* cm, WaypointsManager* wayPointsManager) {
	_robot = robot;
	_wayPointsManager = wayPointsManager;
	_localization_manager = l_manager;
	_cm = cm;

	behavior = pln->getStartPoint();
}
void Manager::run()
{
	wayPoint wp;
	set<wayPoint>::iterator it;

	// Reading data from robot
	_robot->Read();

	// Gets current position of the robot
	double x_Coordinate = _robot->getXpos();
	double y_Coordinate = _robot->getYpos();
	double dTeta = _robot->getYaw();

	// Got through all way points
	for (it = (_wayPointsManager->wayPoints).begin(); it != (_wayPointsManager->wayPoints).end(); ++it) {

		wp = *it;
		_robot->Read();

		_wayPointsManager->setNextWayPoint(wp);

		behavior->startCond();

		// While we haven't got to the needed way point keep moving there
		while (true){


			// If the current behavior can't run, execute the next behavior
			if(behavior->stopCond())
			{
				// Perform the next behavior according to the plan
				behavior = behavior->selectNextBehavior();

			    _robot->Read();

			    // if we hit the way point break, and go on to the next way point
				if (_wayPointsManager->isInWayPoint(_robot->getXpos(),_robot->getYpos()))
				{
					break;
				}

				if (!behavior)
					break;
			}

			// Do the current behavior's action
			behavior->action();

			_robot->Read();

			// Gets the position of the robot after read
			double current_x_coordinate = _robot->getXpos();
			double current_y_coordinate = _robot->getYpos();
			double current_teta = _robot->getYaw();

			double deltaX = current_x_coordinate - x_Coordinate;
			double deltaY = current_y_coordinate - y_Coordinate;
			double deltaTeta = current_teta - dTeta;

			// Update particles
			_localization_manager->update(x_Coordinate, y_Coordinate, dTeta,
										  deltaX, deltaY, deltaTeta, _robot->getLaser());

			x_Coordinate = current_x_coordinate;
			y_Coordinate = current_y_coordinate;
			dTeta = current_teta;
		}
	}
}

Manager::~Manager() {

}
