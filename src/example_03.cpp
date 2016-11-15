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
#include "BeizerPatch.h"
#include "dirLight.h"

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265 // Should be used from mathlib

using namespace std;

/*
For UC Berkeley's CS184 Fall 2016 course, assignment 3 (Bezier surfaces)
*/

//****************************************************
// Global Variables
//****************************************************
GLfloat translation[3] = {0.0f, 0.0f, 1.0f};
GLfloat scale = 1.0f;
GLfloat SCALING_FACTOR = 1.25F;
GLfloat theta[3] = {0.0f, 0.0f, 0.0f};
bool auto_strech = false;
int Width_global = 400;
int Height_global = 400;
int Z_buffer_bit_depth = 128;
string inputfile_name;

inline float sqr(float x) { return x * x; }

double SUB_DIV_PARAM = 0.1;
//dirLight pt = dirLight(Vec3(1,1,1),Color(1,1,1));
vector<BeizerPatch> bzs;


//****************************************************
// MODE SELECTOR
//****************************************************
/*
 * 0 : Naive countour
 * 1 : uniform tessellation wire form
 * 2 : uniform tessellation diffuse
 * 3 : uniform tess wire && deffuse
 * 4 : adaptive tess wire form
 * 5 : adaptive tess wire form
 * 6 : adaptive tess wire form
 */
int MODE_SELECTOR = 2;

/*
 * 0 : FLAT
 * 1 : SMOOTH
 */
int FLAT_SMOOTH = 0;

/*
 * 0 : arch.bez
 * 1 : teacup.bez
 * 2 : teapot.bez
 */
int render_obj = 2;

//****************************************************
// Simple init function
//****************************************************
void initializeRendering() {
    glfwInit();
}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************
void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {
    glColor3f(r, g, b);
    glVertex2f(x + 0.5, y + 0.5);  // The 0.5 is to target pixel centers
    // Note: Need to check for gap bug on inst machines.
}

//****************************************************
// Keyboard inputs. Add things to match the spec! 
//****************************************************
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (key) {

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_LEFT :
            if (action && mods == GLFW_MOD_SHIFT) translation[0] -= 0.001f * Width_global;
            else if(action) {
                theta[1] += 15.0f;
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action && mods == GLFW_MOD_SHIFT) translation[0] += 0.001f * Width_global;
            else if(action) {
                theta[1] -= 15.0f;
            }
            break;
        case GLFW_KEY_UP   :
            if (action && mods == GLFW_MOD_SHIFT) translation[1] += 0.001f * Height_global;
            else if(action) {
                theta[0] -= 15.0f;
            }
            break;
        case GLFW_KEY_DOWN :
            if (action && mods == GLFW_MOD_SHIFT) translation[1] -= 0.001f * Height_global;
            else if(action) {
                theta[0] += 15.0f;
            }
            break;
        case GLFW_KEY_F:
            if (action && mods == GLFW_MOD_SHIFT) auto_strech = !auto_strech;
            break;
        case GLFW_KEY_SPACE:
            break;
        case GLFW_KEY_W : // Filled vs. Wireframe Mode
            cout << "mode_number?" << MODE_SELECTOR << endl;
            if(action){
                if (MODE_SELECTOR == 1) { MODE_SELECTOR = 2; }
                else if (MODE_SELECTOR == 2) { MODE_SELECTOR = 1; }
                else if (MODE_SELECTOR == 3) { MODE_SELECTOR = 1; }
            }
            break;
        case GLFW_KEY_H: // Filled vs. Hidden-line Mode
            if(action){
                if (MODE_SELECTOR < 3) { MODE_SELECTOR = 3; }
                else if (MODE_SELECTOR == 3) { MODE_SELECTOR = 2; }
                else if (MODE_SELECTOR > 3) MODE_SELECTOR = 6;
                else if (MODE_SELECTOR == 6) { MODE_SELECTOR = 4; }
            }
            break;

        case GLFW_KEY_S: // Flat vs. Smooth Shading
            if(action){
                FLAT_SMOOTH = (!FLAT_SMOOTH);
            }
            break;

        case GLFW_KEY_EQUAL: // Zoom In
            if(action){
                scale *= SCALING_FACTOR;
            }
            break;
        case GLFW_KEY_MINUS: // Zoom Out
            if(action){
                scale /= SCALING_FACTOR;
            }
            break;
        default:
            break;
    }

}

//****************************************************
// Draw a cube. You don't need this for your final assignment, but it's
// here so you don't look at a blank screen.
// Taken from https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
//****************************************************
void drawCube() {
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
}

//****************************************************
// Difuse Shader
//****************************************************
//Vec3 diffuse_comp(Vec3 nor){
//    //nor & ray_dir already normalized
//    Vec3 refle = Vec3(1,1,1);
//    Vec3 result = Vec3();
//
//    //iterate through directional light
//
//        GLfloat d = (pt.direction) * nor;
//        Vec3 comp = d * pt.color.co;
//
//        result += comp.indi_scale(refle);
//
//
//
//    return result;
//}

//****************************************************
// Bezier interpretation
//****************************************************
int bezcurveinterp(vector<Vec3> &curve, double u, Vec3 *P, Vec3 *dPdu) {
    Vec3 A = Vec3(curve.at(0) * (1.0 - u) + curve.at(1) * u);
//    cout<<"cv @ 0"<<curve.at(1)._str()<<endl;
    Vec3 B = Vec3(curve.at(1) * (1.0 - u) + curve.at(2) * u);
    Vec3 C = Vec3(curve.at(2) * (1.0 - u) + curve.at(3) * u);
//    cout<<B._str()<<"B?"<<endl;

    //split AB & BC to DE
    Vec3 D(A * (1.0 - u) + B * u);
    Vec3 E(B * (1.0 - u) + C * u);
    //picked point on curve
//    cout<<D._str()<<"D?"<<endl;
//    cout<<E._str()<<"E?"<<endl;

    *P = Vec3(D * (1.0 - u) + E * u);
//    cout<<P->_str()<<"P?"<<endl;

    //derivative
    *dPdu = Vec3((E - D) * 3);
//    cout<<dPdu->_str()<<"dpdu?"<<endl;

    return 0;
}

int bezpatchinterp(BeizerPatch &bz, double u, double v, Vec3 *p, Vec3 *n) {
    vector<Vec3> vcurve, ucurve, temp_c;
    int place = 0;
    //v component
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; k++) {
//            cout<<place<<"place?"<<endl;
            temp_c.push_back(bz.points.at(place));
            place++;
        }
        Vec3 dpdv = Vec3(), v0 = Vec3();
        bezcurveinterp(temp_c, u, &v0, &dpdv);
        vcurve.push_back(v0);
        temp_c.clear();
    }
    //u component
    place = 0;
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
//            cout<<place<<"place2?"<<endl;
            temp_c.push_back(bz.points.at(place));

            place += 4;
        }
        Vec3 dpdu = Vec3(), u0 = Vec3();
        bezcurveinterp(temp_c, v, &u0, &dpdu);
        place = (place + 1) % 16;
        ucurve.push_back(u0);
        temp_c.clear();
    }

    //interpret curve
    Vec3 dPdv = Vec3(), dPdu = Vec3();
    bezcurveinterp(vcurve, v, p, &dPdv);
    bezcurveinterp(ucurve, u, p, &dPdu);

    *n = dPdu.cross(dPdv);
    if((*n).is_zero()){
        *n = dPdv;
    }
    n->normal();

    return 0;
}

void beizerContourPatch(BeizerPatch bz) {


    for (double u = 0; u < 1.001; u += 0.1) {
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(1);
        for (double v = 0; v < 1.001; v += 0.1) {
            Vec3 point = Vec3(), n = Vec3();
            bezpatchinterp(bz, u, v, &point, &n);
            glVertex3f(point.x, point.y, point.z);
//            cout<<"v:"<<point._str()<<endl;
        }
//        cout<<"uv pos:"<<u<<" "<<endl;
        glEnd();
    }

}

int beizerContour() {
    vector<BeizerPatch>::iterator bz_unit = bzs.begin();
    while (bz_unit != bzs.end()) {
        beizerContourPatch(*bz_unit);
        bz_unit++;
    }
    return 0;
}

int tessellateSinglePatch(BeizerPatch &bz) {
    double step_size = SUB_DIV_PARAM;
    for (double t_hor = 0; t_hor < 1.001; t_hor += step_size) {
        glBegin(GL_LINE_STRIP);
//        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glPolygonMode(GL_FRONT, GL_LINE);
        glColor3f(1.0f, 1.0f, 1.0f);
//        glLineWidth(1);
        for (double t_ver = 0; t_ver < 1.001; t_ver += step_size) {
            Vec3 A1 = Vec3(), B2 = Vec3();//,B1 = Vec3(),C = Vec3();
            Vec3 nor = Vec3();
            bezpatchinterp(bz, t_hor, t_ver, &A1, &nor);
            bezpatchinterp(bz, t_hor + step_size, t_ver, &B2, &nor);
            glVertex3f(A1.x, A1.y, A1.z);
            glVertex3f(B2.x, B2.y, B2.z);
        }
        glEnd();
    }
    return 0;
}

int tessellateSinglePatchV2(BeizerPatch &bz) {
    double step_size = SUB_DIV_PARAM;
    for (double t_hor = 0; t_hor < 1.001; t_hor += step_size) {
        glBegin(GL_LINE_STRIP);
//        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glPolygonMode(GL_FRONT, GL_LINE);
        glColor3f(1.0f, 1.0f, 1.0f);
//        glLineWidth(1);
        for (double t_ver = 0; t_ver < 1.001; t_ver += step_size) {
            Vec3 A1 = Vec3(), B2 = Vec3(), B1 = Vec3(), C = Vec3();
            Vec3 nor = Vec3();
            bezpatchinterp(bz, t_hor, t_ver, &A1, &nor);
            bezpatchinterp(bz, t_hor + step_size, t_ver, &B2, &nor);
            bezpatchinterp(bz, t_hor, t_ver + step_size, &B1, &nor);
            bezpatchinterp(bz, t_hor + step_size, t_ver + step_size, &C, &nor);
            glVertex3f(A1.x, A1.y, A1.z);
            glVertex3f(B2.x, B2.y, B2.z);
            glVertex3f(B1.x, B1.y, B1.z);
            glVertex3f(A1.x, A1.y, A1.z);
            glVertex3f(B2.x, B2.y, B2.z);
            glVertex3f(C.x, C.y, C.z);
            glVertex3f(B1.x, B1.y, B1.z);
        }
        glEnd();
    }
    return 0;
}


int tessellateSinglePatchfill(BeizerPatch &bz) {
    double step_size = SUB_DIV_PARAM;
    for (double t_hor = 0; t_hor < 1.001; t_hor += step_size) {
        glBegin(GL_TRIANGLES);
//        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glPolygonMode(GL_FRONT, GL_LINE);
        for (double t_ver = 0; t_ver < 1.001; t_ver += step_size) {
            Vec3 A1 = Vec3(), B2 = Vec3(), B1 = Vec3(), C = Vec3();
            Vec3 nor = Vec3(), norB2 = Vec3(), nor3 = Vec3(), nor4 = Vec3();

            bezpatchinterp(bz, t_hor, t_ver + step_size, &B1, &nor3);
            bezpatchinterp(bz, t_hor + step_size, t_ver + step_size, &C, &nor4);
            bezpatchinterp(bz, t_hor + step_size, t_ver, &B2, &norB2);
            bezpatchinterp(bz, t_hor, t_ver, &A1, &nor);

            //tr1
//            Vec3 color = diffuse_comp(nor);
//            glColor3f(color.x,color.y,color.z);
            glNormal3f(nor.x, nor.y, nor.z);
            glVertex3f(A1.x, A1.y, A1.z);
            glNormal3f(nor3.x, nor3.y, nor3.z);
            glVertex3f(B1.x, B1.y, B1.z);
            glNormal3f(norB2.x, norB2.y, norB2.z);
            glVertex3f(B2.x, B2.y, B2.z);
            //tr2
//            color = diffuse_comp(nor2);
//            glColor3f(color.x,color.y,color.z);
            glNormal3f(norB2.x, norB2.y, norB2.z);
            glVertex3f(B2.x, B2.y, B2.z);
            glNormal3f(nor3.x, nor3.y, nor3.z);
            glVertex3f(B1.x, B1.y, B1.z);
            glNormal3f(nor4.x, nor4.y, nor4.z);
            glVertex3f(C.x, C.y, C.z);


        }
        glEnd();
    }
    return 0;
}

int uniformTessellation() {
    vector<BeizerPatch>::iterator bz_unit = bzs.begin();
    if (MODE_SELECTOR == 1) {
        while (bz_unit != bzs.end()) {
            glDisable(GL_LIGHTING);
            tessellateSinglePatchV2(*bz_unit);
            glEnable(GL_LIGHTING);
            bz_unit++;
        }
    } else if (MODE_SELECTOR == 2) {
        while (bz_unit != bzs.end()) {
            tessellateSinglePatchfill(*bz_unit);
            bz_unit++;
        }
    } else if (MODE_SELECTOR == 3) {
        while (bz_unit != bzs.end()) {
            tessellateSinglePatchfill(*bz_unit);
            glDisable(GL_LIGHTING);
            tessellateSinglePatchV2(*bz_unit);
            glEnable(GL_LIGHTING);
            bz_unit++;
        }
    }

    return 0;
}

int adaptiveTessellation() {

}

//****************************************************
// Matrix trans for teapot
//***************************************************
void teapot_mat(){
    glPushMatrix();
    glTranslatef(0.0f, -.4f, 1.0f);
    glRotatef(215, 1, 0, 0); //rotates the cube below
//    glTranslatef (0.0, 0.0, 0.0);

//    drawCube(); // REPLACE ME!
    if (!MODE_SELECTOR) {
        beizerContour();
    } else if (MODE_SELECTOR >= 1 && MODE_SELECTOR <= 3) {
        uniformTessellation();
    }
    glPopMatrix();
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void display(GLFWwindow *window) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //clear background screen to black


    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    //设置光照
    GLfloat ambient[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position[] = {-1.0, 1.0, -1.0, 0.0};
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);  //  Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION, position);  // Position
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    //设置GL_MODE
    if (FLAT_SMOOTH) {
        glShadeModel(GL_SMOOTH);
    } else {
        glShadeModel(GL_FLAT);

    }
    glLoadIdentity();                            // make sure transformation is "zero'd"
    //----------------------- code to draw objects --------------------------
    glPushMatrix();
    glRotatef(theta[0], 1, 0, 0); //rotates the cube below
    glRotatef(theta[1], 0, 1, 0); //rotates the cube below
    glTranslatef(translation[0], translation[1], translation[2]);
    glScalef(scale, scale, scale);

    teapot_mat();
    glPopMatrix();
    glfwSwapBuffers(window);

    // note: check out glPolygonMode and glShadeModel 
    // for wireframe and shading commands

}

//****************************************************
// function that is called when window is resized
//***************************************************
void size_callback(GLFWwindow *window, int width, int height) {
    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    glfwGetFramebufferSize(window, &Width_global, &Height_global);

    glViewport(0, 0, Width_global, Height_global);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Width_global, 0, Height_global, 1, -1);

    display(window);
}

//****************************************************
// reading from file
//****************************************************
int readinfile() {
    printf("reading obj from file: %s \n", inputfile_name.c_str());
    ifstream infile(inputfile_name.c_str(), ios::in);

    int num_patches;
    char buffer[200];
    if (!infile.is_open()) {
        cout << "wrong reading file" << endl;
        return 1;
    }
    infile.getline(buffer, 200, '\n');
    num_patches = atoi(buffer);
    cout << num_patches << endl;
    while (num_patches) {
        BeizerPatch bz = BeizerPatch();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; ++j) {
                float a, b, c;
                infile >> a >> b >> c;
                bz.points.push_back(Vec3(a, b, c));
            }
        }
        bzs.push_back(bz);
        num_patches--;
    }

    //output test
//    for(int i = 0;i<bzs.size();i++){
//        vector<Vec3> vv(bzs.at(i).points);
//        for(int k = 0;k<16;k++){
//            cout<<vv.at(k)._str()<<"  ";
//        }
//        cout<<"\n";
//    }
    infile.close();
    return 0;
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    //reading file
    int i = 3;
    //reading the file name
    inputfile_name = argv[1];
    readinfile();//reading from file
    SUB_DIV_PARAM = atof(argv[2]);
    while (i < argc) {
        if (!strcmp(argv[i], "-o")) {
            //designating output file name
            i++;
            string name(argv[i]);
//            OUTPUT_FILE = name + ".ppm";

        } else if (0) {

        } else {
            i++;
        }

    }

    //This initializes glfw
    initializeRendering();

    GLFWwindow *window = glfwCreateWindow(Width_global, Height_global, "CS184", NULL, NULL);
    if (!window) {
        cerr << "Error on window creating" << endl;
        glfwTerminate();
        return -1;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!mode) {
        cerr << "Error on getting monitor" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    glfwGetFramebufferSize(window, &Width_global, &Height_global);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.5, 3.5, -3.5, 3.5, 5, -5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);    // enable z-buffering
    glDepthFunc(GL_LESS);

    glfwSetWindowTitle(window, "CS184");
    glfwSetWindowSizeCallback(window, size_callback);
    glfwSetKeyCallback(window, key_callback);


    while (!glfwWindowShouldClose(window)) // infinite loop to draw object again and again
    {   // because once object is draw then window is terminated
        display(window);

        if (auto_strech) {
            glfwSetWindowSize(window, mode->width, mode->height);
            glfwSetWindowPos(window, 0, 0);
        }

        glfwPollEvents();

    }

    return 0;
}

