#include <math.h>
#include "carthesian.h"

double euclidean_distance(struct point p1, struct point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}