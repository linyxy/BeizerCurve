
#include "Vec3.h"


Vec3::Vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}

Vec3::Vec3() : x(0.0), y(0.0), z(0.0) {}

void Vec3::zero() {
    x = y = z = 0;
}

bool Vec3::is_zero() {
    if(x==0 && y==0 && z==0){
        return true;
    }
    return false;
}

Vec3 Vec3::operator-() const {
    return Vec3(-x,-y,-z);
}

Vec3 Vec3::operator+(Vec3 va) const {
    return Vec3(x+va.x,y+va.y,z+va.z);
}

Vec3& Vec3::operator+=(const Vec3 &va) {
    *this = *this + va;
    return *this;
}

Vec3 Vec3::operator-(Vec3 va) const {
    return Vec3(x-va.x,y-va.y,z-va.z);
}

Vec3 Vec3::operator-=(Vec3 &va) {
    *this = *this - va;
    return *this;
}

Vec3 Vec3::operator*(GLfloat m) const {
    return Vec3(x*m,y*m,z*m);
}

GLfloat Vec3::operator*(Vec3 va) const {
    return x*va.x+y*va.y+z*va.z;
}

Vec3 Vec3::operator/(GLfloat m) const {
    return Vec3(x/m, y/m, z/m);
}


Vec3& Vec3::operator*=(GLfloat m) {
    return *this = *this * m;
}

GLfloat Vec3::length() const {
    return (GLfloat)sqrt(x*x+y*y+z*z);
}

void Vec3::normal() {
    GLfloat  l = length();
    GLfloat t ;
    if(l==0)t = 0;
    else  t = 1 / length();

    x = x * t;
    y = y * t;
    z = z * t;
}

//Vec3 Vec3::normal() {
//    GLfloat  l = length();
//    GLfloat t ;
//    if(l==0)t = 0;
//    else  t = 1 / length();
//
//    x = x * t;
//    y = y * t;
//    z = z * t;
//    return Vec3(x, y, z);
//}

Vec3 Vec3::indi_scale(Vec3 &va) const {
    return Vec3(x*va.x,y*va.y,z*va.z);
}

GLfloat Vec3::dist(Vec3 &va) const {
    return (*this - va).length();
}

Vec3 Vec3::cross(Vec3 &va) const {
    return Vec3(this->y * va.z - this->z * va.y,
                this->z * va.x - this->x * va.z,
                this->x * va.y - this->y * va.x);
}


Vec3 operator*(GLfloat m, Vec3 &va) {
    return Vec3(va.x*m, va.y*m, va.z*m);
}

void Vec3::to_str() const {
    cout<<"<"<<this->x<<","<<this->y<<","<<this->z<<"> ";
}

string Vec3::_str() {
    return "<"+to_string(this->x)+","+to_string(this->y)+","+to_string(this->z)+">";
}