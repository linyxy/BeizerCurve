//
// Created by Linyxy on 16/11/15.
//

#include "BzPoint.h"

BzPoint::BzPoint(const Vec3 &pos, double u, double v, const Vec3 &normal) : pos(pos), u(u), v(v), normal(normal) {}

BzPoint::BzPoint() {
    pos = Vec3();
    u = v = 0;
}
