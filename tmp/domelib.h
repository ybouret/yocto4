#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if defined(__linux__)
   //#include <GL/glew.h>
   #include <GL/glut.h>
#elif defined(__APPLE__)
   #include <GLUT/glut.h>
#else
   #warning "Not sure where to find glut.h"
#endif
#include "paulslib.h"
#include "bitmaplib.h"
#include "opengllib.h"

// Comment this out if frame buffer objects are not available
#define TRYFBO TRUE

// This defines a single node of the distorting mesh.
typedef struct {
   double x,y;         // Coordinate
   double u,v;         // UV coordinate
   double i;           // Intensity
} WARPMESHNODE;

// A single triangle of the cube face mesh
// Should change to a shared vertex mesh except the performance so far isn't a problem
typedef struct {
   double u[3],v[3];    // Texture coordinates for fisheye
   XYZ p[3];            // Vertices for fisheye
} DOMEFACE;

typedef struct {

	// Adjusted through Dome_Modify()
	int hflip,vflip; // true or false
   double aperture; // degrees
   XYZ offset; // -1 ... 1
	int showcubemesh;
	int showfishmesh;
	int showintensity;

   // Mesh for fisheye, supplied in Dome_Initialise
	void (*drawfcn)();
	int debug;
   WARPMESHNODE **mesh;
   int meshnx,meshny;

	// Type of texture mapping
   int texturetype;

	// Cubic map mesh
	int cuberesol; // Number of times to refine the dome, eg: 3,4,5
	int ncubetop, ncubebottom;
	int ncubeleft, ncuberight;
	DOMEFACE *cubetop,*cubebottom;
	DOMEFACE *cubeleft,*cuberight;

	// Cubic textures
	int cubesize;
	BITMAP4 *texturetop,*texturebottom;
	BITMAP4 *textureleft,*textureright;
	GLuint textureid[4];

	// Fisheye
	BITMAP4 *fishimage;
	int fishsize;
	GLuint fishtextureid;
   GLuint fbo;
   GLuint depthbuffer;
	int fbostatus;

} DOME_OPTIONS;

int  Dome_Initialise(void *,char *,int);
void Dome_DefaultMesh(void);
void Dome_NodeUV(double,double,int,int);
void Dome_NodeXY(XYZ);
int  Dome_ReadMesh(char *);
int  Dome_CubeCreate(void);
void Dome_CubeSplitFace(DOMEFACE *,int *);
void Dome_CubeFlatten(XYZ *,double);
void Dome_FishDraw(int);
int  Dome_FishTexture(void);
int  Dome_CubeTexture(void);
void Dome_Display(XYZ,XYZ,XYZ,int,int,double,double);
int  Dome_Modify(XYZ,double,int,int,int,int,int);
void Dome_OpenglState(void);
void Dome_Info(int *,int *);

// Play around
void SaveObj(char *,DOMEFACE *,int);

