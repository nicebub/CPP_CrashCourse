#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "hippomocks.h"
#include "autobrake.h"
#include "iservicebusinterface.h"

#include <stdexcept>
#include <string>
TEST_CASE("AutoBrake") {
	MockRepository mocks;
	mocks.autoExpect = false;
	CarDetectedCallback car_detected_callback;
	SpeedUpdateCallback speed_update_callback;
	SpeedLimitCallback speed_limit_callback;
	BrakeCommand last_command{};
	int commands_published{};
	auto* bus = mocks.Mock<IServiceBus>();
	mocks.ExpectCall(bus, IServiceBus::subscribe_to_speed)
		.Do([&](const auto& x) {
			speed_update_callback = x;
		});

	mocks.ExpectCall(bus, IServiceBus::subscribe_to_car_detected)
		.Do([&](const auto& x) {
			car_detected_callback = x;
	});

	mocks.ExpectCall(bus, IServiceBus::subscribe_to_speed_limit)
		.Do([&](const auto& x) {
			speed_limit_callback = x;
	});

	AutoBrake auto_brake { *bus };

	SECTION("Initializes speed to zero") {
		REQUIRE(auto_brake.get_speed_mps() == Approx(0));
	}
	
	SECTION("Initializes sensitivity to five") {
		REQUIRE(auto_brake.get_collision_threshold_s() == Approx(5));
	}
	SECTION("throws when sensitivity less than one") {
		REQUIRE_THROWS(auto_brake.set_collision_threshold_s(0.5L));
	}
	
	SECTION("Initialize last known speed limit to 39"){
		REQUIRE(auto_brake.get_last_speed_limit_mps() == Approx(39));
	}
	
	SECTION("saves speed after update") {
		speed_limit_callback(SpeedLimitDetected{100});
		speed_update_callback(SpeedUpdate{ 100L });
		REQUIRE(100L == auto_brake.get_speed_mps());
		speed_update_callback(SpeedUpdate{ 50L});
		REQUIRE(50L == auto_brake.get_speed_mps());
		speed_update_callback(SpeedUpdate{ 0L});
		REQUIRE(0L == auto_brake.get_speed_mps());
	}

	SECTION("saves last known speed limit detected") {
		speed_limit_callback(SpeedLimitDetected{ 5L });
		REQUIRE(5L == auto_brake.get_last_speed_limit_mps());
		speed_limit_callback(SpeedLimitDetected{ 50L });
		REQUIRE(50L == auto_brake.get_last_speed_limit_mps());
		speed_limit_callback(SpeedLimitDetected{ 70L });
		REQUIRE(70L == auto_brake.get_last_speed_limit_mps());
	}

	SECTION("no alert when not imminent") {
		auto_brake.set_collision_threshold_s(2L);
		speed_limit_callback(SpeedLimitDetected{100});
		speed_update_callback(SpeedUpdate{ 100L});
		car_detected_callback(CarDetected{ 1000L, 50L});
		REQUIRE(commands_published == 0 );
	}
	
	SECTION("alert when imminent") {
		mocks.ExpectCall(bus, IServiceBus::publish)
			.Match([](const auto& cmd){
				return cmd.time_to_collision_s == Approx(1);
			})
			.Do([&](const auto& x) {
				commands_published++;
				last_command = x;
			});
		auto_brake.set_collision_threshold_s( 10L);
		speed_limit_callback(SpeedLimitDetected{100});
		speed_update_callback(SpeedUpdate{ 100L});
		car_detected_callback(CarDetected{ 100L, 0L});
		CAPTURE(commands_published, last_command.time_to_collision_s);
		REQUIRE(commands_published == 1);
		REQUIRE(last_command.time_to_collision_s == Approx(1));
	}
	
	SECTION("last known speed limit is 35 going at 34 no Brake Command"){
		speed_update_callback(SpeedUpdate{ 34L});
		REQUIRE(34L == auto_brake.get_speed_mps());
		speed_limit_callback(SpeedLimitDetected{ 35L });
		REQUIRE(35L == auto_brake.get_last_speed_limit_mps());
		REQUIRE(commands_published == 0 );
		CAPTURE(commands_published, last_command.time_to_collision_s);
	}
	SECTION("last known speed limit is 35 going at 40 Brake Command sent once"){
		mocks.ExpectCall(bus, IServiceBus::publish)
			.Match([](const auto& cmd){
				return cmd.time_to_collision_s == Approx(0);
			})
			.Do([&](const auto& x) {
				commands_published++;
				last_command = x;
			});
		speed_limit_callback(SpeedLimitDetected{ 35L });
		REQUIRE(35L == auto_brake.get_last_speed_limit_mps());
		speed_update_callback(SpeedUpdate{ 40L});
		REQUIRE(40L == auto_brake.get_speed_mps());
		CAPTURE(commands_published, last_command.time_to_collision_s);
		REQUIRE(commands_published == 1);
		REQUIRE(last_command.time_to_collision_s == Approx(0));
	}
	SECTION("last known speed limit is 35 going at 30 then Speed changes"
		" to 25 Brake Command sent once"){
		mocks.ExpectCall(bus, IServiceBus::publish)
			.Match([](const auto& cmd){
				return cmd.time_to_collision_s == Approx(0);
			})
			.Do([&](const auto& x) {
				commands_published++;
				last_command = x;
			});
		speed_limit_callback(SpeedLimitDetected{ 35L });
		REQUIRE(35L == auto_brake.get_last_speed_limit_mps());
		speed_update_callback(SpeedUpdate{ 30L});
		REQUIRE(30L == auto_brake.get_speed_mps());
		speed_limit_callback(SpeedLimitDetected{ 25L });
		REQUIRE(25L == auto_brake.get_last_speed_limit_mps());
		CAPTURE(commands_published, last_command.time_to_collision_s);
		REQUIRE(commands_published == 1);
		REQUIRE(last_command.time_to_collision_s == Approx(0));
	}
}
