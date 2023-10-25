#pragma once

#ifndef MATH_H
#define MATH_H

#include <SDL/SDL_rect.h>

template <typename T>
inline T random() {
	return (T)(double)rand() / (double)RAND_MAX;
}

template <typename T>
inline T random(T max) {
	return (T)((double)rand() / (double)RAND_MAX * (double)max);
}

template <typename T>
inline T random(T min, T max) {
	double t = (double)rand() / (double)RAND_MAX;
	return (T)((double)min * t + (1.0 - t) * (double)max);
}

inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) {
	return {a.x + b.x, a.y + b.y};
}

inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) {
	return {a.x - b.x, a.y - b.y};
}

inline SDL_FPoint operator*(const SDL_FPoint& a, const SDL_FPoint& b) {
	return {a.x * b.x, a.y * b.y};
}

inline SDL_FPoint operator*(const SDL_FPoint& a, const float& factor) {
	return {a.x * factor, a.y * factor};
}

template <typename T>
inline float magnitude_squared(const T& point) {
	return point.x * point.x + point.y * point.y;
}

inline bool check_collision(SDL_FPoint obj1, SDL_FPoint obj2, float r1, float r2) {
	return magnitude_squared(obj2 - obj1) <= ((r1 + r2) * (r1 + r2));
}

#endif