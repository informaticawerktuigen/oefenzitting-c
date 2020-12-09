#ifndef CARTHESIAN_H
#define CARTHESIAN_H

struct point
{
    int x;
    int y;
};

double euclidean_distance(struct point p1, struct point p2);

#endif