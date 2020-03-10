#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "exercise1.hpp"
#include <optional>
#include <limits>

TEST_CASE("testing std::optional narrow_cast"){
	int a;
	SECTION("narrow_cast returns same on not narrowing"){
		a = 4;
		const auto c = narrow_cast<short>(a);
		REQUIRE(a == c.value());
	}
	SECTION("narrow_cast returns std::nulopt if narrowing occurs"){
		a = std::numeric_limits<int>::max();
		const auto c = narrow_cast<short>(a);
		REQUIRE(c == std::nullopt);
	}
}