#include "includes.h"
#include "math.h"

float calculate_delta_time(float* lf)
{
    float cf = (float)glfwGetTime();
    float dt = cf - *lf;
    *lf = cf;
    return dt;
}

float clamp_f(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}
int clamp_i(int v, int min, int max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}
double clamp_d(double v, double min, double max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}