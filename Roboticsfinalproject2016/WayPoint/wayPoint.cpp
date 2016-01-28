/*
 * wayPoint.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: colman
 */

#include "wayPoint.h"

wayPoint::wayPoint(double x, double y){

	this->x= x;
	this->y = y;
}
wayPoint::wayPoint() {

	this->x = 0;
	this->y = 0;
}


wayPoint::~wayPoint() {
}

