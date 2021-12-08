#include "functions.h"

int manhattan_distance(int x1, int x2, int y1, int y2)
{
    return absolute_value(x2 - x1) + absolute_value(y2 - y1);
}

int main(void)
{
    return manhattan_distance(10, 0, 0, 10);
}