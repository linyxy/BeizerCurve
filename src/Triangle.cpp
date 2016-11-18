//
// Created by Linyxy on 16/10/24.
//

#include "Triangle.h"






Triangle::Triangle() {
    p1 = Vec3();
    p2 = Vec3();
    p3 = Vec3();
}

Triangle::Triangle(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3) : p1(p1), p2(p2), p3(p3) {

    normal = (p1-p2).cross(Vec3(p1-p3));
    normal.normal();
}

void Triangle::draw_f(){

    glBegin(GL_TRIANGLES);
    GLfloat gl_normal[] = {normal.x, normal.y, normal.z};
    glNormal3fv(gl_normal);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glVertex3f(p3.x, p3.y, p3.z);
    glEnd();
}


void Triangle::draw_s(){

    glBegin(GL_TRIANGLES);

    GLfloat gl_normal_p1[] = {p1_normal.x, p1_normal.y, p1_normal.z};
    glNormal3fv(gl_normal_p1);
    glVertex3f(p1.x, p1.y, p1.z);

    GLfloat gl_normal_p2[] = {p2_normal.x, p2_normal.y, p2_normal.z};
    glNormal3fv(gl_normal_p2);
    glVertex3f(p2.x, p2.y, p2.z);

    GLfloat gl_normal_p3[] = {p3_normal.x, p3_normal.y, p3_normal.z};
    glNormal3fv(gl_normal_p3);
    glVertex3f(p3.x, p3.y, p3.z);

    glEnd();
}

//
//bool Triangle::intersect(Ray &ray, float *thit, Intersection *in, BRDF *intensities) {
//    Vec3 e1 = p2-p1;
//    Vec3 e2 = p3-p1;
//    Vec3 P = ray.dir.cross(e2);
//
//    //det
//    float det = e1*P;
//    Vec3 T;
//    if( det > 0){
//        T = (ray.pos-Point()) - p1;
//    } else{
//        T = p1 - (ray.pos-Point());
//        det = -det;
//    }
//
//    if(det < 0.0001f) return false;
//
//    float u = T*P;
//    if(u < 0.0 || u > det)return false;
//
//    Vec3 Q = T.cross(e1);
//    float v = ray.dir*Q;
//    if(v< 0.0 || v+u > det)return false;
//
//    float t = e2*Q;
//    float invDet = 1.0 / det;
//    t *= invDet;
//    if(t<ray.t_min || t>ray.t_max)
//        return false;
//
//    *thit = t;
//    Point inter = ray.pos+(*thit)*ray.dir;
//    Vec3 nor = e1.cross(e2);
//    Vec3 to_ray = -ray.dir;
////    to_ray.normal();
//    nor.normal();
////    if(nor*to_ray<(-nor)*to_ray){
////        nor = -nor;
////    }
//
//    in->localGeo = LocalGeo(inter,nor);
//
//    *intensities = brdf;
//    return true;
//}
//
//bool Triangle::intersectT(Ray &ray, float *thit) {
//    Vec3 e1 = p2-p1;
//    Vec3 e2 = p3-p1;
//    Vec3 P = ray.dir.cross(e2);
//
//    //det
//    float det = e1*P;
//    Vec3 T;
//    if( det > 0){
//        T = ray.pos.pos - p1;
//    } else{
//        T = p1 - ray.pos.pos;
//        det = -det;
//    }
//
//    if(det < 0.0001f) return false;
//
//    float u = T*P;
//    if(u < 0.0 || u > det)return false;
//
//    Vec3 Q = T.cross(e1);
//    float v = ray.dir*Q;
//    if(v< 0.0 || v+u > det)return false;
//
//    float t = e2*Q;
//    float invDet = 1.0 / det;
//    t *= invDet;
//    if(t<ray.t_min || t>ray.t_max)
//        return false;
//    *thit = t;
//    return true;
//}