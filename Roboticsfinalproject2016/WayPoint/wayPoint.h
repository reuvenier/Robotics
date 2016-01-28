
#ifndef WAYPOINT_H_
#define WAYPOINT_H_

#include "limits.h"

class wayPoint {
public:
	double x;
	double y;


	wayPoint();
	wayPoint(double x, double y);

	virtual ~wayPoint();

	bool operator<(const wayPoint& wp) const{

			return (y*INT_MAX + x > wp.y*INT_MAX + wp.x);
		}
};

#endif /* WAYPOINT_H_ */
