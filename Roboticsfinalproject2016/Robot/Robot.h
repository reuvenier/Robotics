
#ifndef ROBOT_H_
#define ROBOT_H_

#include <libplayerc++/playerc++.h>
#include "../Defines.h"
#include "../Managers/ConfigurationManager.h"

using namespace PlayerCc;

class Robot {

	PlayerClient* _pc;
	Position2dProxy* _pp;
	LaserProxy* _lp;
	ConfigurationManager* _cm;

public:

	double _x;
	double _y;
	double _yaw;
	int _grid_rows;

	Robot(char* ip, int port, ConfigurationManager* cm,int grid_rows);
	void Read();
	void setSpeed(float xSpeed, float angularSpeed);
	bool isRightFree();
	bool isLeftFree();
	bool isForwardFree();
	LaserProxy* getLaser();
	double getXpos();
	double getYpos();
	double getYaw();
	double mapToWorldX(int x);
	double mapToWorldY(int y);
	double WorldTomapX(int x);
	double WorldTomapY(int y);
	void setX(double x);
	void setY(double y);
	void setYaw(double yaw);
	float getLaserDistance(int index);
	bool checkRange(int nStart, int nEnd);
	double getLaserSpec();

	static double degToRadians (double deg) {
			return (deg * M_PI) / 180;
		}

	static double radiansToDeg (double rad) {
			return (rad * 180) / M_PI;
		}

	virtual ~Robot();
};

#endif /* ROBOT_H_ */
