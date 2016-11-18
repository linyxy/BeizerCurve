//
// Created by Linyxy on 16/10/24.
//

#include <vector>
#include <fstream>
#include <cmath>

//include header file for glfw library so that we can use OpenGL
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>

//include matrix
//#include <glm/glm.hpp>


#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H


#include "Vec3.h"


class Triangle {
public:
    Vec3 p1,p2,p3;
    Vec3 normal;
    Vec3 p1_normal;
    Vec3 p2_normal;
    Vec3 p3_normal;

//    Triangle(const )

    Triangle();

    Triangle(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3);

//    bool intersectP(Ray& ray);
    /*
     * triangle intersection  used "picking" method
     * reference from (Chinese translation)http://www.cnblogs.com/graphics/archive/2010/08/09/1795348.html
     * original paper: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
     */
//    bool intersect(Ray& ray, float* thit, Intersection* in,BRDF* intensities);
//    bool intersectT(Ray& ray, float* thit);
public:
    void draw_f();
    void draw_s();

};


#endif //RAYTRACER_TRIANGLE_H
