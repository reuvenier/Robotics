
#ifndef ROBOT_H_
#define ROBOT_H_

#include <libplayerc++/playerc++.h>
#include "../Managers/ConfigurationManager.h"
#include "../Map/Map.h"

using namespace PlayerCc;

typedef pair<double, double> Cell;
// The robot class is managing the robot, it gets the way points vector,
// and know how to get to the next one.
//
class Robot {
public:
	static const double kStartYaw = 0.79;
	static const double kDistanceTolerance = 0.15;
	static const double kTurnLeftYaw = 0.3;
	static const double kTurnRightYaw= -0.3;
	static const double kForwardSpeed = 0.2;

private:
	PlayerClient player_client_;
	Position2dProxy position_proxy_;
	LaserProxy laser_proxy_;
	ConfigurationManager* config_manager;
	vector<Node *>  way_points_;
	unsigned int next_way_point_index_;



public:
	Robot(const char* ip, int port, ConfigurationManager* config_manager);

	bool FreeInFront(float distance);

	//If we got to the destination, next_way_point_index
	// will be incremented by one.
	bool CheckIfReachToDestination();

	// Moves the robot to the next way point.
	void GoToNextWaypoint();

	double GetLaser(int index);
	const vector<Node*>& GetWayPoints() const;

	void SetSpeed(float x_speed, float y_speed);
	void SetWayPoints(const vector<Node*>& wayPoints);
	void Read(){player_client_.Read();}

	Cell ConvertMapToReal(int x, int y);
	Cell ConvertRealToMap(int x, int y);

	double MapToWorldX(int x);
	double MapToWorldY(int y);
	double WorldTomapX(int x);
	double WorldTomapY(int y);

	double RadiansToDeg(double rad);

	virtual ~Robot();
};

#endif /* ROBOT_H_ */
