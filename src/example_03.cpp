#include <vector>
#include <fstream>
#include <cmath>

//include header file for glfw library so that we can use OpenGL
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>

//include matrix
#include <glm/glm.hpp>
#include "BeizerPatch.h"

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
GLfloat translation[3] = {1.0f, 0.0f, 0.0f};
bool auto_strech = false;
int Width_global = 400;
int Height_global = 400;
int Z_buffer_bit_depth = 128;
string inputfile_name;
inline float sqr(float x) { return x*x; }
vector<BeizerPatch> bzs;

//****************************************************
// Simple init function
//****************************************************
void initializeRendering()
{
    glfwInit();
}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************
void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {
    glColor3f(r, g, b);
    glVertex2f(x+0.5, y+0.5);  // The 0.5 is to target pixel centers
    // Note: Need to check for gap bug on inst machines.
}

//****************************************************
// Keyboard inputs. Add things to match the spec! 
//****************************************************
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
            
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_LEFT :
            if (action && mods == GLFW_MOD_SHIFT) translation[0] -= 0.001f * Width_global; break;
        case GLFW_KEY_RIGHT:
            if (action && mods == GLFW_MOD_SHIFT) translation[0] += 0.001f * Width_global; break;
        case GLFW_KEY_UP   :
            if (action && mods == GLFW_MOD_SHIFT) translation[1] += 0.001f * Height_global; break;
        case GLFW_KEY_DOWN :
            if (action && mods == GLFW_MOD_SHIFT) translation[1] -= 0.001f * Height_global; break;
        case GLFW_KEY_F:
            if (action && mods == GLFW_MOD_SHIFT) auto_strech = !auto_strech; break;
        case GLFW_KEY_SPACE: break;
            
        default: break;
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
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
      // Bottom face (y = -1.0f)
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 
      // Front face  (z = 1.0f)
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      // Back face (z = -1.0f)
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);
 
      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      // Right face (x = 1.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();  // End of drawing color-cube
}


//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void display( GLFWwindow* window )
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    
    //----------------------- code to draw objects --------------------------
    glPushMatrix();
    glTranslatef (translation[0], translation[1], translation[2]);
    glRotatef(45, 1, 1, 0); //rotates the cube below
    drawCube(); // REPLACE ME!

    glPopMatrix();
    
    glfwSwapBuffers(window);

    // note: check out glPolygonMode and glShadeModel 
    // for wireframe and shading commands
    
}

//****************************************************
// function that is called when window is resized
//***************************************************
void size_callback(GLFWwindow* window, int width, int height)
{
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
int readinfile(){
    printf("reading obj from file: %s \n", inputfile_name.c_str());
    ifstream infile(inputfile_name.c_str(),ios::in);

    int num_patches;
    char buffer[200];
    if(!infile.is_open()){
        cout<<"wrong reading file"<<endl;
        return 1;
    }
    infile.getline(buffer,200,'\n');
    num_patches = atoi(buffer);
    cout<<num_patches<<endl;
    while(num_patches){
        BeizerPatch bz = BeizerPatch();
        for(int i = 0;i<4;i++){
            for (int j = 0; j < 4; ++j) {
                float a,b,c;
                infile >> a >> b >> c;
                bz.points.push_back(Vec3(a,b,c));
            }
        }
        bzs.push_back(bz);
        num_patches--;
    }

    //output test
    for(int i = 0;i<bzs.size();i++){
        vector<Vec3> vv(bzs.at(i).points);
        for(int k = 0;k<16;k++){
            cout<<vv.at(k)._str()<<"  ";
        }
        cout<<"\n";
    }
    infile.close();
    return 0;
}

//****************************************************
// Bezier interpretation
//****************************************************
int bezcurveinterp(vector<Vec3> &curve, double u,Vec3* P,Vec3* dPdu){
    Vec3 A = curve.at(0) * (1.0-u) + curve.at(1) * u;
    Vec3 B = curve.at(1) * (1.0-u) + curve.at(2) * u;
    Vec3 C = curve.at(2) * (1.0-u) + curve.at(3) * u;
    //split AB & BC to DE
    Vec3 D(A * (1.0-u) + B * u);
    Vec3 E(B * (1.0-u) + C * u);
    //picked point on curve
    *P = (D * (1.0-u) + E * u);
    //derivative
    *dPdu = ( (E - D) * 3);
    return 0;
}

int bezpatchinterp(BeizerPatch &bz,double u, double v,Vec3* p,Vec3* n){
    vector<Vec3> vcurve,ucurve,temp_c;
    int place = 0;
    //v component
    for (int i = 0; i < 4; ++i) {
        for(int k = 0;k<4;k++ ) {
            temp_c.push_back(bz.points.at(place));
            place++;
        }
        Vec3 dpdv(0,0,0),v0(0,0,0);
        bezcurveinterp(temp_c,u,&v0,&dpdv);
        vcurve.push_back(v0);
        temp_c.clear();
    }
    //u component
    for(int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
            temp_c.push_back(bz.points.at(place));
            place+=4;
        }
        Vec3 dpdu(0,0,0),u0(0,0,0);
        bezcurveinterp(temp_c,v,&u0,&dpdu);
        place = (place+5)%16;
        ucurve.push_back(u0);
        temp_c.clear();
    }

    //interpret curve
    Vec3 dPdv(0,0,0),dPdu(0,0,0);
    bezcurveinterp(vcurve,v,p,&dPdv);
    bezcurveinterp(ucurve,v,p,&dPdu);

    *n = dPdu.cross(dPdv);
    n->normal();

    return 0;
}



//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    //reading file
    int i = 1;
    while (i < argc) {
        {
            //reading the file name
            inputfile_name =  argv[1];
            readinfile();//reading from file
            i++;
        }
        if (!strcmp(argv[i], "-o")) {
            //designating output file name
            i++;
            string name(argv[i]);
//            OUTPUT_FILE = name + ".ppm";

        } else if(0){

        }

    }

    //This initializes glfw
    initializeRendering();
    
    GLFWwindow* window = glfwCreateWindow( Width_global, Height_global, "CS184", NULL, NULL );
    if ( !window )
    {
        cerr << "Error on window creating" << endl;
        glfwTerminate();
        return -1;
    }

    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if ( !mode )
    {
        cerr << "Error on getting monitor" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    
    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    glfwGetFramebufferSize(window, &Width_global, &Height_global);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-3.5, 3.5, -3.5, 3.5, 5, -5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glEnable(GL_DEPTH_TEST);	// enable z-buffering
    glDepthFunc(GL_LESS);

    glfwSetWindowTitle(window, "CS184");
    glfwSetWindowSizeCallback(window, size_callback);
    glfwSetKeyCallback(window, key_callback);

    while( !glfwWindowShouldClose( window ) ) // infinite loop to draw object again and again
    {   // because once object is draw then window is terminated
        display( window );
        
        if (auto_strech){
            glfwSetWindowSize(window, mode->width, mode->height);
            glfwSetWindowPos(window, 0, 0);
        }
        
        glfwPollEvents();
        
    }

    return 0;
}

