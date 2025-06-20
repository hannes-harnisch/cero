#pragma once

namespace util {

template<typename>
struct is_flag_enum : std::false_type {};

template<typename T>
requires is_flag_enum<T>::value
T operator&(T a, T b) {
	return static_cast<T>(std::to_underlying(a) & std::to_underlying(b));
}

template<typename T>
requires is_flag_enum<T>::value
T operator|(T a, T b) {
	return static_cast<T>(std::to_underlying(a) | std::to_underlying(b));
}

template<typename T>
requires is_flag_enum<T>::value
T operator^(T a, T b) {
	return static_cast<T>(std::to_underlying(a) ^ std::to_underlying(b));
}

template<typename T>
requires is_flag_enum<T>::value
T operator~(T t) {
	return static_cast<T>(~std::to_underlying(t));
}

template<typename T>
requires is_flag_enum<T>::value
T& operator&=(T& a, T b) {
	return a = a & b;
}

template<typename T>
requires is_flag_enum<T>::value
T& operator|=(T& a, T b) {
	return a = a | b;
}

template<typename T>
requires is_flag_enum<T>::value
T& operator^=(T& a, T b) {
	return a = a ^ b;
}

template<typename T>
requires is_flag_enum<T>::value
bool has_flags(T mask, T flags) {
	return (mask & flags) == flags;
}

} // namespace util
