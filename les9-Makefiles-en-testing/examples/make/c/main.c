#include <stdio.h>
#include "carthesian.h"
#include "print.h"

int main(void)
{
    struct point p1;
    p1.x = 0;
    p1.y = 0;

    struct point p2;
    p2.x = 10;
    p1.y = -10;

    printf("The distance between ");
    print_point(p1);
    printf(" and ");
    print_point(p2);
    printf(" is %f\n", euclidean_distance(p1, p2));
    return 0;
}