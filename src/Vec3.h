//
// Created by Linyxy on 16/9/27.
//

#ifndef ASSIGNMENT1_VEC3_H
#define ASSIGNMENT1_VEC3_H

#include <math.h>
#include <iostream>
using namespace std;

class Vec3
{

public:
    float x,y,z;

public:
    //constructor
    Vec3(float  x, float  y, float  z);
    Vec3();


    //normal vectors
    void zero();
    bool is_zero();
    Vec3 operator-() const;
    void normal();

    float length() const ;
    //重载
    //add
    Vec3 operator+(Vec3 va) const ;
    Vec3& operator+=(const Vec3 &va);
    //sub
    Vec3 operator-(Vec3 va) const;
    Vec3 operator-=(Vec3 &va);
    //scale
    Vec3 operator*(float m) const;
    Vec3& operator*=(float m);

    Vec3 operator/(float m) const;

    //dist
    float  dist(Vec3 &va) const ;
    //dot prodcut
    float  operator*(Vec3 va) const;

    Vec3 indi_scale(Vec3 &va) const;
    //cross product
    Vec3 cross(Vec3 va) const;

    void to_str() const;

    string _str();

};

Vec3 operator*(float m, Vec3 &va);

#endif //ASSIGNMENT1_VEC3_H
