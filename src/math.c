#include "includes.h"
#include "math.h"

float calcdt(float* lf)
{
    float cf = (float)glfwGetTime();
    float dt = cf - *lf;
    *lf = cf;
    return dt;
}

float clampf(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}
int clampi(int v, int min, int max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}
double clampd(double v, double min, double max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}