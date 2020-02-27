#include <functional>

#include "autobrake.h"
#include "iservicebusinterface.h"

	AutoBrake::AutoBrake(IServiceBus& bus) : 
	collision_threshold_s{ 5},
	speed_mps{},
	last_speed_limit_mps { 39 } {
	bus.subscribe_to_speed([this,&bus](const SpeedUpdate& update) {
		speed_mps = update.velocity_mps;
		if(speed_mps > last_speed_limit_mps){
			bus.publish(BrakeCommand{0});
		}
	});
	bus.subscribe_to_car_detected([this, &bus](const CarDetected& cd) {
		const auto relative_velocity_mps = speed_mps - cd.velocity_mps;
		const auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
		if(time_to_collision_s > 0 &&
			time_to_collision_s <= collision_threshold_s) {
				bus.publish(BrakeCommand{ time_to_collision_s});
			}
		});
		bus.subscribe_to_speed_limit([this,&bus](const SpeedLimitDetected & sl) {
			last_speed_limit_mps = sl.speed_mps;
			if(last_speed_limit_mps < speed_mps){
				bus.publish(BrakeCommand{0});
			}
		});
	}	
	void AutoBrake::set_collision_threshold_s(double x) {
		if( x < 1) throw std::runtime_error{ "Collision less than 1." };
		collision_threshold_s = x;
	}
	
	double AutoBrake::get_collision_threshold_s() const {
		return collision_threshold_s;
	}
	double AutoBrake::get_speed_mps() const {
		return speed_mps;
	}

	double AutoBrake::get_last_speed_limit_mps() const {
		return last_speed_limit_mps;
	}