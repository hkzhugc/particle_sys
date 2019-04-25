#include "util.h"
#include <math.h>

double uniform_random()
{
	return (double)std::rand() / (double)RAND_MAX;
}

vec2 uniform_random_vec2()
{
	float X1 = uniform_random();
	float X2 = uniform_random();
	return vec2(X1, X2);
}

vec2 sample_circle()
{
	float r = uniform_random();
	float thetha = uniform_random() * 2 * M_PI;
	return vec2(r * sin(thetha), r * cos(thetha));
}
