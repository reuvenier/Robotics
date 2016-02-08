
#include "Robot.h"
#include <math.h>

Robot::Robot(const char* ip, int port, ConfigurationManager* config_manager) :
player_client_(ip, port), position_proxy_(&player_client_), laser_proxy_(&player_client_) {

	position_proxy_.SetMotorEnable(true);

	double x = 0;
	double y = 0;
	x=MapToWorldX((config_manager->kMapWidth/config_manager->kPlayerMapWidth)*(config_manager->kPlayerMapWidth/2+2.17));
	y=MapToWorldY((config_manager->kMapLength/config_manager->kPlayerMapLength)*(config_manager->kPlayerMapLength/2+2.88));

	cout<<"start position for robot ("<<x<<","<<y<<")"<<endl;
	position_proxy_.SetOdometry(x,y,kStartYaw);
	for (int i = 0; i < 50; i++) {
		this->player_client_.Read();
	}

}

void Robot::SetSpeed(float x_speed, float y_speed) {
	position_proxy_.SetSpeed(x_speed, y_speed);
}

bool Robot::FreeInFront(float distance) {
	int i;
	for (i = 200; i < 400; i += 5){
		if (laser_proxy_[i] < distance){
			return false;
		}
	}

	return true;
}

bool Robot::CheckIfReachToDestination() {

	Position current_position = way_points_[next_way_point_index_]->GetPosition();
	double y = MapToWorldY(current_position.first);
	double x = MapToWorldX(current_position.second);

	double cx = position_proxy_.GetXPos();
	double cy = position_proxy_.GetYPos();
	cout <<"Robot player position : "<< cx << "," << cy << endl;
	if (next_way_point_index_ > way_points_.size()) {
		return true;
	}

	if (sqrt(pow((x - cx), 2) + pow((y - cy), 2)) <= kDistanceTolerance) { // make sure that he is in the WayPoint
		next_way_point_index_++;
	}
	return false;
}

void Robot::GoToNextWaypoint() {
	double x = position_proxy_.GetXPos();
	double y = position_proxy_.GetYPos();
	double yaw = position_proxy_.GetYaw();
	Node* nextWayPoint = way_points_[next_way_point_index_];
	Position nextWaypointPosition = nextWayPoint->GetPosition();
	double cy = MapToWorldY(nextWaypointPosition.first);
	double cx = MapToWorldX(nextWaypointPosition.second);


	double deltaX = cx-x;

	double deltaY = cy-y;
	double s = atan2(deltaY , deltaX);

	cout << "Yaw is " << yaw << endl;


	if (yaw >=0 && s >= 0)  {

		if (yaw > s + 0.1) {

			position_proxy_.SetSpeed(0, kTurnRightYaw);
		} else if (yaw < s - 0.3) {

			position_proxy_.SetSpeed(0, kTurnLeftYaw);
		} else {

			position_proxy_.SetSpeed(kForwardSpeed, 0);
		}
	} else if (yaw < 0 && s < 0) {

		if (yaw > s + 0.3) {

			position_proxy_.SetSpeed(0, kTurnRightYaw);
		} else if (yaw < s - 0.3) {

			position_proxy_.SetSpeed(0, kTurnLeftYaw);
		} else {


			position_proxy_.SetSpeed(kForwardSpeed, 0);
		}
	} else if (yaw >=0 && s < 0) {

		if (3.14 - yaw + 3.14 - abs(s) > yaw + abs(s)) {

			position_proxy_.SetSpeed(0, kTurnRightYaw);
		} else if (3.14 - yaw + 3.14 - abs(s) < yaw + abs(s)) {

			position_proxy_.SetSpeed(0, kTurnLeftYaw);
		} else {

			position_proxy_.SetSpeed(kForwardSpeed, 0);
		}
	} else if (yaw < 0 && s >= 0) {
		if (3.14 - abs(yaw) + 3.14 - s > abs(yaw) + s) {

			position_proxy_.SetSpeed(0,kTurnLeftYaw);
		} else if (3.14 - abs(yaw) + 3.14 - s < abs(yaw) + s) {

			position_proxy_.SetSpeed(0, kTurnRightYaw);
		} else {

			position_proxy_.SetSpeed(kForwardSpeed, 0);
		}
	}


	cout << "next way point: " << next_way_point_index_ << " The point is: " << cx
			<< "," << cy << endl;
}

const vector<Node*>& Robot::GetWayPoints() const {
	return way_points_;
}

void Robot::SetWayPoints(const vector<Node*>& way_points) {
	this->way_points_ = way_points;
	next_way_point_index_ = 0;
}

Cell Robot::ConvertMapToReal(int x, int y){
	double xCell;
	double yCell;
	xCell= (y - (config_manager->kMapWidth / 2)) * 0.025;
	yCell=-(x - (config_manager->kMapLength / 2)) * 0.025;
	return Cell(xCell,yCell);
}
Cell Robot::ConvertRealToMap(int x, int y){
	double xCell;
	double yCell;
	xCell= (x / config_manager->getMapResolution()) + (config_manager->kMapWidth / 2);
	yCell=-((y / config_manager->getMapResolution()) - (config_manager->kMapLength / 2));
	return Cell(xCell,yCell);
}


double Robot::MapToWorldX(int x) {//Y
	return (x - (550 / 2)) * 0.025;
}
double Robot::MapToWorldY(int y) {//X
	return -(y - (380 / 2)) * 0.025;
}

double Robot::WorldTomapX(int x) {
	return (x / 0.025) + (550 / 2);

}
double Robot::WorldTomapY(int y) {
	return -((y / 0.025) - (380 / 2));
}


double Robot::GetLaser(int index){
	return this->laser_proxy_[index];
}

double Robot::RadiansToDeg (double rad) {
	return (rad * 180) / M_PI;
}


Robot::~Robot() {

}
