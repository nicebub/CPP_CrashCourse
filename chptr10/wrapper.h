#ifndef _WRAPPER_H
#define _WRAPPER_H

struct SpeedUpdate {
	double velocity_mps;
};

struct CarDetected {
	double distance_m;
	double velocity_mps;
};

struct BrakeCommand {
	double time_to_collision_s;
};

struct SpeedLimitDetected {
	unsigned short speed_mps;
};

#endif