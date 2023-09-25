#include "../include/RandomFloat.h"

float RandomFloat::generate(float left, float right) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(left, right);
	return dist(gen);
}
