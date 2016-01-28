/*
 * GoToWayPoint.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: colman
 */

#include "GoToWayPoint.h"

GoToWayPoint::GoToWayPoint(Robot* robot, WaypointsManager* wpm):Behavior(robot) {
	_wpm = wpm;
}

GoToWayPoint::~GoToWayPoint() {


}

bool GoToWayPoint::startCond(){
	return _robot->isForwardFree();
}

bool GoToWayPoint::stopCond()
{
	return !_robot->isForwardFree();

}
void GoToWayPoint::action()
{
	    double x = _robot->getXpos();
		double y = _robot->getYpos();
		double yaw = _robot->getYaw();
		double deg = _robot->radiansToDeg(yaw);
		wayPoint nextWayPoint = _wpm->getNextWayPoint();
		double cy = _robot->mapToWorldY(nextWayPoint.y);
		double cx = _robot->mapToWorldX(nextWayPoint.x);


		double deltaX = cx-x;

		double deltaY = cy-y;
		double s = atan2(deltaY , deltaX);

		cout << "Yaw is " << yaw << endl;
		cout << "S is " << s << endl;

		if (yaw >=0 && s >= 0)  {
		    if (yaw > s + 0.1) {
				cout << "a" << endl;
				_robot->setSpeed(0, -0.1);

			} else if (yaw < s - 0.1) {
				cout << "b" << endl;
				_robot->setSpeed(0, 0.1);

			} else {
				cout << "c" << endl;
				_robot->setSpeed(0.2, 0);

			}
		} else if (yaw < 0 && s < 0) {
			if (yaw > s + 0.1) {
				cout << "a" << endl;
				_robot->setSpeed(0, 0.1);

			} else if (yaw < s - 0.1) {
				cout << "b" << endl;
				_robot->setSpeed(0, -0.1);

			} else {
				cout << "c" << endl;
				_robot->setSpeed(0.2, 0);
			}
		} else if (yaw >=0 && s < 0) {
			if (3.14 - yaw + 3.14 - abs(s) > yaw + abs(s)) {
				cout << "a" << endl;
				_robot->setSpeed(0, -0.1);

			} else if (3.14 - yaw + 3.14 - abs(s) < yaw + abs(s)) {
				cout << "b" << endl;
				_robot->setSpeed(0, 0.1);

			} else {
				cout << "c" << endl;
				_robot->setSpeed(0.2, 0);

			}
		} else if (yaw < 0 && s >= 0) {
			if (3.14 - abs(yaw) + 3.14 - s > abs(yaw) + s) {
				cout << "a" << endl;
				_robot->setSpeed(0, 0.1);

			} else if (3.14 - abs(yaw) + 3.14 - s < abs(yaw) + s) {
				cout << "b" << endl;
				_robot->setSpeed(0, -0.1);

			} else {
				cout << "c" << endl;
				_robot->setSpeed(0.2, 0);
			}
		}
}
