//
// Created by Linyxy on 16/9/25.
//

#ifndef ASSIGNMENT1_COLOR_H
#define ASSIGNMENT1_COLOR_H

#include <GLFW/glfw3.h>
#include "Vec3.h"
#include <cmath>



class Color {
    public:
        Vec3 co;
        GLfloat alpha;
    public:
        Color();
        Color(GLfloat r,GLfloat g, GLfloat b,GLfloat alpha = 1);
        Color(Vec3 clr, GLfloat alpha = 1);

        GLfloat R() const ;
        GLfloat G() const;
        GLfloat B() const ;



        Vec3 comp() const;

        //重载
        Color operator*(GLfloat &m) const ;
        void to_str() const;
};


#endif //ASSIGNMENT1_COLOR_H
