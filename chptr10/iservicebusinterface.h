#ifndef _ISERVICEBUS_H
#define _ISERVICEBUS_H
#include "wrapper.h"

using SpeedUpdateCallback = std::function<void(const SpeedUpdate&)>;
using CarDetectedCallback = std::function<void(const CarDetected&)>;
using SpeedLimitCallback = std::function<void(const SpeedLimitDetected&)>;


struct IServiceBus {
	virtual ~IServiceBus() = default;
	virtual void publish(const BrakeCommand&) = 0;
	virtual void subscribe_to_speed(SpeedUpdateCallback) = 0;
	virtual void subscribe_to_car_detected(CarDetectedCallback) = 0;
	virtual void subscribe_to_speed_limit(SpeedLimitCallback) = 0;
};

#endif