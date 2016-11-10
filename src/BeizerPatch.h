//
// Created by Linyxy on 16/11/9.
//

#ifndef ASSIGNMENT1_BEIZERPATCH_H
#define ASSIGNMENT1_BEIZERPATCH_H

#include <GLFW/glfw3.h>
#include <vector>
#include "Vec3.h"

using namespace std;

class BeizerPatch {
public:
    vector<Vec3> points;

    BeizerPatch();
};


#endif //ASSIGNMENT1_BEIZERPATCH_H
