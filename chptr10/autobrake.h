#ifndef _AUTOBRAKE_H
#define _AUTOBRAKE_H
#include "iservicebusinterface.h"
struct AutoBrake {
	AutoBrake(IServiceBus& bus);
	void set_collision_threshold_s(double x);
	double get_collision_threshold_s() const;
	double get_speed_mps() const;
	double get_last_speed_limit_mps() const;

private:
	double collision_threshold_s;
	double speed_mps;
	double last_speed_limit_mps;

};


#endif