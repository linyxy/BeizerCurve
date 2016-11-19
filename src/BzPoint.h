//
// Created by Linyxy on 16/11/15.
//

#ifndef ASSIGNMENT1_BZPOINT_H
#define ASSIGNMENT1_BZPOINT_H


#include "Vec3.h"

class BzPoint {
public:
    Vec3 pos;
    double u,v;

    Vec3 normal;

    BzPoint(const Vec3 &pos, double u, double v, const Vec3 &normal);

    BzPoint();
};


#endif //ASSIGNMENT1_BZPOINT_H
