#ifndef _EX1_H
#define _EX1_H

#include <optional>

template<typename To, typename From>
std::optional<To> narrow_cast(From value){
	const auto converted = static_cast<To>(value);
	const auto backwards = static_cast<From>(converted);
	if(value != backwards) return std::nullopt;
	return converted;
}
#endif