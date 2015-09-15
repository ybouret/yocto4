#include "domelib.h"

// Comment this to remove figure generation
//#define MAKEFIGURES TRUE
//#define MAKEOBJ TRUE

static DOME_OPTIONS dome_options;

/*
	Functions by which the external application interface with this library are:
	Dome_Initialise();
		Must be called once and after the window context exists
		Set geometry call back function
		Choose a mesh file
		Optionally enable debugging
		Assume graphics context exists, eg: GLUT
	Dome_Modify();
		Sets offset, aperture, flipping, mesh displays
	Dome_Info();
		Returns the resolution of the cube and fisheye mesh
	Dome_Display();
		Call this to update the image on the back buffer
		Requires camera position, direction, and up vector in world coordinates
		Ready for a swapbuffer()
*/

/*
	Initialise the dome library
*/
int Dome_Initialise(void (*drawfcn),char *meshfile,int debug)
{
	// Adjustable externally
	dome_options.hflip = FALSE;
	dome_options.vflip = FALSE;
   dome_options.aperture = 180;
   dome_options.offset.x = 0;
   dome_options.offset.y = 0;
   dome_options.offset.z = 0;
	dome_options.showcubemesh = FALSE;
	dome_options.showfishmesh = FALSE;
	dome_options.showintensity = TRUE;

	dome_options.debug = debug;
   dome_options.texturetype = GL_TEXTURE_2D;
	dome_options.cuberesol = 5; // 3 gives artifacts, 6 the maximum
#if MAKEFIGURES
	dome_options.cuberesol = 4;
#endif

	// Cube map faces
	dome_options.cubetop = 0;
	dome_options.cubetop = NULL;
	dome_options.ncubebottom = 0;
   dome_options.cubebottom = NULL;
	dome_options.ncubeleft = 0;
   dome_options.cubeleft = NULL;
	dome_options.ncuberight = 0;
   dome_options.cuberight = NULL;

	// Cubic textures
	dome_options.cubesize = 512;
	dome_options.texturetop = NULL;
	dome_options.texturebottom = NULL;
	dome_options.textureleft = NULL;
	dome_options.textureright = NULL;

	// Fisheye
	dome_options.fishimage = NULL;
	dome_options.fishsize = 512;
	dome_options.fbo = 0;

   // Possibly read the mesh file, otherwise create a default mesh
   if (strlen(meshfile) > 0 && Dome_ReadMesh(meshfile)) {
      ;
   } else if (Dome_ReadMesh("default.data")) {
      ;
   } else {
      Dome_DefaultMesh();
   }

	// Draw function supplied by the user
	dome_options.drawfcn = drawfcn;

   // Can we handle non power of 2 textures?
   #if GL_EXT_texture_rectangle
      dome_options.texturetype = GL_TEXTURE_RECTANGLE_EXT;
   #else
      #if GL_NV_texture_rectangle
         dome_options.texturetype = GL_TEXTURE_RECTANGLE_NV;
      #else
         dome_options.texturetype = GL_TEXTURE_2D;
      #endif
   #endif

	// Try to sensibly determine the size of the cube and fisheye textures
   while (dome_options.cubesize+32 < glutGet(GLUT_SCREEN_WIDTH) && 
			 dome_options.cubesize+32 < glutGet(GLUT_SCREEN_HEIGHT)) 
      dome_options.cubesize += 32;
	dome_options.cubesize -= 128; // Compensate for window decorations, YUK!
	
	// Limit fish and cube size if powers of 2 are required
	if (dome_options.texturetype == GL_TEXTURE_2D) {
		if (dome_options.cubesize < 512)
			dome_options.cubesize = 256;
		else if (dome_options.cubesize < 1024)
			dome_options.cubesize = 512;
		else
			dome_options.cubesize = 1024;
	}
	if (dome_options.debug)
		fprintf(stderr,"Cube texture size: %d, Bounds: %d,%d\n",
			dome_options.cubesize,glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT));

	// Create the cubic mesh
	if (!Dome_CubeCreate()) {
      if (dome_options.debug)
         fprintf(stderr,"Failed to create cubic faces\n");
		return(FALSE);
	}

	// Set up the cubic textures
	if (!Dome_CubeTexture()) {
      if (dome_options.debug)
         fprintf(stderr,"Failed to cubic textures\n");
		return(FALSE);
	}

	// Set up the fisheye texture
	dome_options.fishsize = dome_options.cubesize; // Initial guess without FBO
	if (!Dome_FishTexture())
		return(FALSE);

   // Create fisheye display lists
   Dome_FishDraw(TRUE);

	return(TRUE);
}

/*
	Use this to modify internals
	Appropriate things are updated, like recreating the cube mesh and displaylists
*/
int Dome_Modify(XYZ offset,double aperture,int hflip,int vflip,int cmesh,int fmesh,int showi)
{
   dome_options.aperture = aperture;
   dome_options.offset = offset;
	dome_options.hflip = hflip;
	dome_options.vflip = vflip;
	dome_options.showcubemesh = cmesh;
	dome_options.showfishmesh = fmesh;
	dome_options.showintensity = showi;

   // Create the cubic mesh
   if (!Dome_CubeCreate()) {
		if (dome_options.debug)
			fprintf(stderr,"Failed to create cubic faces\n");
      return(FALSE);
	}

	// Refresh the display lists for the cubic maps
	Dome_FishDraw(TRUE);

	return(TRUE);
}

/*
	Use this to get useful information the application may need
*/
void Dome_Info(int *ngrid,int *nfish)
{
	*ngrid = dome_options.cubesize;
	*nfish = dome_options.fishsize;
}

/*
	Create a default, rectangular mesh, results in a fisheye
*/
void Dome_DefaultMesh(void)
{
	int i,j;

	// Size
   dome_options.meshnx = 40;
   dome_options.meshny = 30;

   // Memory
   dome_options.mesh = malloc(dome_options.meshnx*sizeof(WARPMESHNODE *));
   for (i=0;i<dome_options.meshnx;i++)
      dome_options.mesh[i] = malloc(dome_options.meshny*sizeof(WARPMESHNODE));

   // Contents
   for (j=0;j<dome_options.meshny;j++) {
      for (i=0;i<dome_options.meshnx;i++) {
         dome_options.mesh[i][j].x = 2 * i / (double)(dome_options.meshnx-1) - 1;
         dome_options.mesh[i][j].y = 2 * j / (double)(dome_options.meshny-1) - 1;
         dome_options.mesh[i][j].u = i / (double)(dome_options.meshnx-1);
         dome_options.mesh[i][j].v = j / (double)(dome_options.meshny-1);
         dome_options.mesh[i][j].i = 1;
      }
   }
}

/*
   Set a node UV depending on texture mode
   u,v are in normalise coordinates
*/
void Dome_NodeUV(double u,double v,int width,int height)
{
   if (dome_options.texturetype == GL_TEXTURE_2D)
      glTexCoord2f(u,v);
   else
      glTexCoord2f(u*width,v*height);
}

/*
	Set a vertex taking into account flipping flags
	z is up, x is horizontal
*/
void Dome_NodeXY(XYZ p)
{
   if (dome_options.vflip)
      p.z *= -1;
   if (dome_options.hflip)
      p.x *= -1;
   glVertex3f(p.x,p.y,p.z);
}

/*
	Read a mesh file
   First line is the dimensions
   Subsequent lines are the mesh nodes: x,y,u,v,i
*/
int Dome_ReadMesh(char *fname)
{
   int i,j;
   int meshtype;
   double x,y,u,v,br;
   double xmin = 1e32,ymin = 1e32,xmax = -1e32,ymax = -1e32;
   FILE *fptr;

   // Attempt to open the file
   if ((fptr = fopen(fname,"r")) == NULL) 
      return(FALSE);

   // Get the mesh type
   if (fscanf(fptr,"%d",&meshtype) != 1) {
		if (dome_options.debug)
      	fprintf(stderr,"Failed to read the mesh type\n");
      fclose(fptr);
      return(FALSE);
   }
   if (meshtype < 0 || meshtype > 5) {
		if (dome_options.debug)
      	fprintf(stderr,"Failed to get a recognised map type (%d)\n",meshtype);
      fclose(fptr);
      return(FALSE);
   }

   // Get the dimensions
   if (fscanf(fptr,"%d %d",&dome_options.meshnx,&dome_options.meshny) != 2) {
		if (dome_options.debug)
      	fprintf(stderr,"Failed to read the mesh dimensions\n");
      fclose(fptr);
      return(FALSE);
   }
   if (dome_options.meshnx < 4 || dome_options.meshny < 4 || 
		 dome_options.meshnx > 100000 || dome_options.meshny > 100000) {
		if (dome_options.debug)
      	fprintf(stderr,"Didn't read acceptable mesh resolution (%d,%d)\n",
				dome_options.meshnx,dome_options.meshny);
      fclose(fptr);
      return(FALSE);
   }

   // Create new mesh
   dome_options.mesh = malloc(dome_options.meshnx*sizeof(WARPMESHNODE *));
   for (i=0;i<dome_options.meshnx;i++)
      dome_options.mesh[i] = malloc(dome_options.meshny*sizeof(WARPMESHNODE));

   // Read the values
   for (j=0;j<dome_options.meshny;j++) {
      for (i=0;i<dome_options.meshnx;i++) {
         if (fscanf(fptr,"%lf %lf %lf %lf %lf",&x,&y,&u,&v,&br) != 5) {
				if (dome_options.debug)
            	fprintf(stderr,"Unexpected end of mesh file encountered, fatal\n");
         	x = 2 * i / (double)(dome_options.meshnx-1) - 1;
         	y = 2 * j / (double)(dome_options.meshny-1) - 1;
         	u = i / (double)(dome_options.meshnx-1);
         	v = j / (double)(dome_options.meshny-1);
         	br = 1;
         }
         xmin = MIN(xmin,x);
         ymin = MIN(ymin,y);
         xmax = MAX(xmax,x);
         ymax = MAX(ymax,y);
         dome_options.mesh[i][j].x = x;
         dome_options.mesh[i][j].y = y;
         dome_options.mesh[i][j].u = u;
         dome_options.mesh[i][j].v = v;
         dome_options.mesh[i][j].i = br;
      }
   }

   fclose(fptr);
   return(TRUE);
}

/*
   Create a facet representation of the dome disk to a particular resolution
   Sets all the correct texture coordinates
   Deal with offset projection
   This is all done algorithmically rather than a final mathematical equation
   Dome disk is in the x-z plane at y=0
   The camera is lacated at (0,-1,0), see HandleDIsplay()
              | z
              |
              |
              |
              +--------- y
             /
            /
           / x
*/
int Dome_CubeCreate(void)
{
   int i,j;

#ifdef MAKEFIGURES
	int k;
	char fname[64];
	FILE *fptr;
#endif

   /* Top face - just a triangle */
   dome_options.cubetop = realloc(dome_options.cubetop,sizeof(DOMEFACE));
   dome_options.cubetop[0].p[0].x = -sqrt(2.0) / 2.0;
   dome_options.cubetop[0].p[0].y = 0;
   dome_options.cubetop[0].p[0].z = 0.5;
   dome_options.cubetop[0].u[0] = 0;
   dome_options.cubetop[0].v[0] = 1;
   dome_options.cubetop[0].p[1].x = 0;
   dome_options.cubetop[0].p[1].y = sqrt(2.0) / 2.0;
   dome_options.cubetop[0].p[1].z = 0.5;
   dome_options.cubetop[0].u[1] = 0;
   dome_options.cubetop[0].v[1] = 0;
   dome_options.cubetop[0].p[2].x = sqrt(2.0) / 2.0;
   dome_options.cubetop[0].p[2].y = 0;
   dome_options.cubetop[0].p[2].z = 0.5;
   dome_options.cubetop[0].u[2] = 1;
   dome_options.cubetop[0].v[2] = 0;
   dome_options.ncubetop = 1;

   /* Bottom face - just a triangle */
   dome_options.cubebottom = realloc(dome_options.cubebottom,sizeof(DOMEFACE));
   dome_options.cubebottom[0].p[0].x = -sqrt(2.0) / 2.0;
   dome_options.cubebottom[0].p[0].y = 0;
   dome_options.cubebottom[0].p[0].z = -0.5;
   dome_options.cubebottom[0].u[0] = 1;
   dome_options.cubebottom[0].v[0] = 0;
   dome_options.cubebottom[0].p[1].x = sqrt(2.0) / 2.0;
   dome_options.cubebottom[0].p[1].y = 0;
   dome_options.cubebottom[0].p[1].z = -0.5;
   dome_options.cubebottom[0].u[1] = 0;
   dome_options.cubebottom[0].v[1] = 1;
   dome_options.cubebottom[0].p[2].x = 0;
   dome_options.cubebottom[0].p[2].y = sqrt(2.0) / 2.0;
   dome_options.cubebottom[0].p[2].z = -0.5;
   dome_options.cubebottom[0].u[2] = 0;
   dome_options.cubebottom[0].v[2] = 0;
   dome_options.ncubebottom = 1;

   /* Left face - two triangles */
   dome_options.cubeleft = realloc(dome_options.cubeleft,2*sizeof(DOMEFACE));
   dome_options.cubeleft[0].p[0].x = -sqrt(2.0) / 2.0;
   dome_options.cubeleft[0].p[0].y = 0;
   dome_options.cubeleft[0].p[0].z = -0.5;
   dome_options.cubeleft[0].u[0] = 0;
   dome_options.cubeleft[0].v[0] = 0;
   dome_options.cubeleft[0].p[1].x = 0;
   dome_options.cubeleft[0].p[1].y = sqrt(2.0) / 2.0;
   dome_options.cubeleft[0].p[1].z = -0.5;
   dome_options.cubeleft[0].u[1] = 1;
   dome_options.cubeleft[0].v[1] = 0;
   dome_options.cubeleft[0].p[2].x = -sqrt(2.0) / 2.0;
   dome_options.cubeleft[0].p[2].y = 0;
   dome_options.cubeleft[0].p[2].z = 0.5;
   dome_options.cubeleft[0].u[2] = 0;
   dome_options.cubeleft[0].v[2] = 1;
   dome_options.cubeleft[1].p[0].x = -sqrt(2.0) / 2.0;
   dome_options.cubeleft[1].p[0].y = 0;
   dome_options.cubeleft[1].p[0].z = 0.5;
   dome_options.cubeleft[1].u[0] = 0;
   dome_options.cubeleft[1].v[0] = 1;
   dome_options.cubeleft[1].p[1].x = 0;
   dome_options.cubeleft[1].p[1].y = sqrt(2.0) / 2.0;
   dome_options.cubeleft[1].p[1].z = -0.5;
   dome_options.cubeleft[1].u[1] = 1;
   dome_options.cubeleft[1].v[1] = 0;
   dome_options.cubeleft[1].p[2].x = 0;
   dome_options.cubeleft[1].p[2].y = sqrt(2.0) / 2.0;
   dome_options.cubeleft[1].p[2].z = 0.5;
   dome_options.cubeleft[1].u[2] = 1;
   dome_options.cubeleft[1].v[2] = 1;
   dome_options.ncubeleft = 2;

   /* Right face - two triangles */
   dome_options.cuberight  = realloc(dome_options.cuberight,2*sizeof(DOMEFACE));
   dome_options.cuberight[0].p[0].x = 0;
   dome_options.cuberight[0].p[0].y = sqrt(2.0) / 2.0;
   dome_options.cuberight[0].p[0].z = -0.5;
   dome_options.cuberight[0].u[0] = 0;
   dome_options.cuberight[0].v[0] = 0;
   dome_options.cuberight[0].p[1].x = sqrt(2.0) / 2.0;
   dome_options.cuberight[0].p[1].y = 0;
   dome_options.cuberight[0].p[1].z = -0.5;
   dome_options.cuberight[0].u[1] = 1;
   dome_options.cuberight[0].v[1] = 0;
   dome_options.cuberight[0].p[2].x = sqrt(2.0) / 2.0;
   dome_options.cuberight[0].p[2].y = 0;
   dome_options.cuberight[0].p[2].z = 0.5;
   dome_options.cuberight[0].u[2] = 1;
   dome_options.cuberight[0].v[2] = 1;
   dome_options.cuberight[1].p[0].x = 0;
   dome_options.cuberight[1].p[0].y = sqrt(2.0) / 2.0;
   dome_options.cuberight[1].p[0].z = -0.5;
   dome_options.cuberight[1].u[0] = 0;
   dome_options.cuberight[1].v[0] = 0;
   dome_options.cuberight[1].p[1].x = sqrt(2.0) / 2.0;
   dome_options.cuberight[1].p[1].y = 0;
   dome_options.cuberight[1].p[1].z = 0.5;
   dome_options.cuberight[1].u[1] = 1;
   dome_options.cuberight[1].v[1] = 1;
   dome_options.cuberight[1].p[2].x = 0;
   dome_options.cuberight[1].p[2].y = sqrt(2.0) / 2.0;
   dome_options.cuberight[1].p[2].z = 0.5;
   dome_options.cuberight[1].u[2] = 0;
   dome_options.cuberight[1].v[2] = 1;
   dome_options.ncuberight = 2;

   /*
      Refine the faces to the desired resolution
      Achieved by splitting each triangles into 4
      Preserve texture coordinates (of course)
      Possibly draw the top face at higher resolution for mirror warp maps
   */
   for (i=0;i<dome_options.cuberesol;i++) {
#ifdef MAKEFIGURES
		sprintf(fname,"cube%d.geom",i);
		fptr = fopen(fname,"w");
		for (k=0;k<dome_options.ncubetop;k++) 
			fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
				dome_options.cubetop[k].p[0].x,dome_options.cubetop[k].p[0].y,dome_options.cubetop[k].p[0].z,
            dome_options.cubetop[k].p[1].x,dome_options.cubetop[k].p[1].y,dome_options.cubetop[k].p[1].z,
            dome_options.cubetop[k].p[2].x,dome_options.cubetop[k].p[2].y,dome_options.cubetop[k].p[2].z);
		for (k=0;k<dome_options.ncubebottom;k++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubebottom[k].p[0].x,dome_options.cubebottom[k].p[0].y,dome_options.cubebottom[k].p[0].z,
            dome_options.cubebottom[k].p[1].x,dome_options.cubebottom[k].p[1].y,dome_options.cubebottom[k].p[1].z,
            dome_options.cubebottom[k].p[2].x,dome_options.cubebottom[k].p[2].y,dome_options.cubebottom[k].p[2].z);
		for (k=0;k<dome_options.ncubeleft;k++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubeleft[k].p[0].x,dome_options.cubeleft[k].p[0].y,dome_options.cubeleft[k].p[0].z,
            dome_options.cubeleft[k].p[1].x,dome_options.cubeleft[k].p[1].y,dome_options.cubeleft[k].p[1].z,
            dome_options.cubeleft[k].p[2].x,dome_options.cubeleft[k].p[2].y,dome_options.cubeleft[k].p[2].z);
		for (k=0;k<dome_options.ncuberight;k++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cuberight[k].p[0].x,dome_options.cuberight[k].p[0].y,dome_options.cuberight[k].p[0].z,
            dome_options.cuberight[k].p[1].x,dome_options.cuberight[k].p[1].y,dome_options.cuberight[k].p[1].z,
            dome_options.cuberight[k].p[2].x,dome_options.cuberight[k].p[2].y,dome_options.cuberight[k].p[2].z);
		fclose(fptr);
#endif
      dome_options.cubetop = realloc(dome_options.cubetop,(4*dome_options.ncubetop)*sizeof(DOMEFACE));
      Dome_CubeSplitFace(dome_options.cubetop,&dome_options.ncubetop);
      dome_options.cubebottom = realloc(dome_options.cubebottom,(4*dome_options.ncubebottom)*sizeof(DOMEFACE));
      Dome_CubeSplitFace(dome_options.cubebottom,&dome_options.ncubebottom);
      dome_options.cubeleft = realloc(dome_options.cubeleft,(4*dome_options.ncubeleft)*sizeof(DOMEFACE));
      Dome_CubeSplitFace(dome_options.cubeleft,&dome_options.ncubeleft);
      dome_options.cuberight = realloc(dome_options.cuberight,(4*dome_options.ncuberight)*sizeof(DOMEFACE));
      Dome_CubeSplitFace(dome_options.cuberight,&dome_options.ncuberight);
   }

   /* Turn into a hemisphere */
   for (j=0;j<3;j++) {
      for (i=0;i<dome_options.ncubetop;i++)
         Normalise(&(dome_options.cubetop[i].p[j]));
      for (i=0;i<dome_options.ncubebottom;i++)
         Normalise(&(dome_options.cubebottom[i].p[j]));
      for (i=0;i<dome_options.ncubeleft;i++)
         Normalise(&(dome_options.cubeleft[i].p[j]));
      for (i=0;i<dome_options.ncuberight;i++)
         Normalise(&(dome_options.cuberight[i].p[j]));
   }
#ifdef MAKEFIGURES
      sprintf(fname,"dome.geom");
      fptr = fopen(fname,"w");
      for (i=0;i<dome_options.ncubetop;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubetop[i].p[0].x,dome_options.cubetop[i].p[0].y,dome_options.cubetop[i].p[0].z,
            dome_options.cubetop[i].p[1].x,dome_options.cubetop[i].p[1].y,dome_options.cubetop[i].p[1].z,
            dome_options.cubetop[i].p[2].x,dome_options.cubetop[i].p[2].y,dome_options.cubetop[i].p[2].z);
		for (i=0;i<dome_options.ncubebottom;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubebottom[i].p[0].x,dome_options.cubebottom[i].p[0].y,dome_options.cubebottom[i].p[0].z,
            dome_options.cubebottom[i].p[1].x,dome_options.cubebottom[i].p[1].y,dome_options.cubebottom[i].p[1].z,
            dome_options.cubebottom[i].p[2].x,dome_options.cubebottom[i].p[2].y,dome_options.cubebottom[i].p[2].z);
		for (i=0;i<dome_options.ncubeleft;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubeleft[i].p[0].x,dome_options.cubeleft[i].p[0].y,dome_options.cubeleft[i].p[0].z,
            dome_options.cubeleft[i].p[1].x,dome_options.cubeleft[i].p[1].y,dome_options.cubeleft[i].p[1].z,
            dome_options.cubeleft[i].p[2].x,dome_options.cubeleft[i].p[2].y,dome_options.cubeleft[i].p[2].z);
		for (i=0;i<dome_options.ncuberight;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cuberight[i].p[0].x,dome_options.cuberight[i].p[0].y,dome_options.cuberight[i].p[0].z,
            dome_options.cuberight[i].p[1].x,dome_options.cuberight[i].p[1].y,dome_options.cuberight[i].p[1].z,
            dome_options.cuberight[i].p[2].x,dome_options.cuberight[i].p[2].y,dome_options.cuberight[i].p[2].z);
      fclose(fptr);
#endif

   /* Apply offaxis shift */
   for (j=0;j<3;j++) {
      for (i=0;i<dome_options.ncubetop;i++) {
         dome_options.cubetop[i].p[j].x += dome_options.offset.x;
         dome_options.cubetop[i].p[j].z += dome_options.offset.z;
         Normalise(&(dome_options.cubetop[i].p[j]));
      }
      for (i=0;i<dome_options.ncubebottom;i++) {
         dome_options.cubebottom[i].p[j].x += dome_options.offset.x;
         dome_options.cubebottom[i].p[j].z += dome_options.offset.z;
         Normalise(&(dome_options.cubebottom[i].p[j]));
      }
      for (i=0;i<dome_options.ncubeleft;i++) {
         dome_options.cubeleft[i].p[j].x += dome_options.offset.x;
         dome_options.cubeleft[i].p[j].z += dome_options.offset.z;
         Normalise(&(dome_options.cubeleft[i].p[j]));
      }
      for (i=0;i<dome_options.ncuberight;i++) {
         dome_options.cuberight[i].p[j].x += dome_options.offset.x;
         dome_options.cuberight[i].p[j].z += dome_options.offset.z;
         Normalise(&(dome_options.cuberight[i].p[j]));
      }
   }

   // Flatten onto xz plane
   for (j=0;j<3;j++) {
      for (i=0;i<dome_options.ncubetop;i++) 
         Dome_CubeFlatten(&(dome_options.cubetop[i].p[j]),dome_options.aperture);
      for (i=0;i<dome_options.ncubebottom;i++) 
         Dome_CubeFlatten(&(dome_options.cubebottom[i].p[j]),dome_options.aperture);
      for (i=0;i<dome_options.ncubeleft;i++) 
         Dome_CubeFlatten(&(dome_options.cubeleft[i].p[j]),dome_options.aperture);
      for (i=0;i<dome_options.ncuberight;i++) 
         Dome_CubeFlatten(&(dome_options.cuberight[i].p[j]),dome_options.aperture);
   }
#ifdef MAKEFIGURES
      sprintf(fname,"flat.geom");
      fptr = fopen(fname,"w");
      for (i=0;i<dome_options.ncubetop;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubetop[i].p[0].x,dome_options.cubetop[i].p[0].y,dome_options.cubetop[i].p[0].z,
            dome_options.cubetop[i].p[1].x,dome_options.cubetop[i].p[1].y,dome_options.cubetop[i].p[1].z,
            dome_options.cubetop[i].p[2].x,dome_options.cubetop[i].p[2].y,dome_options.cubetop[i].p[2].z);
		for (i=0;i<dome_options.ncubebottom;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubebottom[i].p[0].x,dome_options.cubebottom[i].p[0].y,dome_options.cubebottom[i].p[0].z,
            dome_options.cubebottom[i].p[1].x,dome_options.cubebottom[i].p[1].y,dome_options.cubebottom[i].p[1].z,
            dome_options.cubebottom[i].p[2].x,dome_options.cubebottom[i].p[2].y,dome_options.cubebottom[i].p[2].z);
		for (i=0;i<dome_options.ncubeleft;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cubeleft[i].p[0].x,dome_options.cubeleft[i].p[0].y,dome_options.cubeleft[i].p[0].z,
            dome_options.cubeleft[i].p[1].x,dome_options.cubeleft[i].p[1].y,dome_options.cubeleft[i].p[1].z,
            dome_options.cubeleft[i].p[2].x,dome_options.cubeleft[i].p[2].y,dome_options.cubeleft[i].p[2].z);
		for (i=0;i<dome_options.ncuberight;i++)
         fprintf(fptr,"f3 %g %g %g %g %g %g %g %g %g 0 0 0\n",
            dome_options.cuberight[i].p[0].x,dome_options.cuberight[i].p[0].y,dome_options.cuberight[i].p[0].z,
            dome_options.cuberight[i].p[1].x,dome_options.cuberight[i].p[1].y,dome_options.cuberight[i].p[1].z,
            dome_options.cuberight[i].p[2].x,dome_options.cuberight[i].p[2].y,dome_options.cuberight[i].p[2].z);
      fclose(fptr);
#endif

	// Save to OBJ files for external texture wrapping to fisheye, eg for Unity
	#ifdef MAKEOBJ
		SaveObj("left.obj",dome_options.cubeleft,dome_options.ncubeleft);
   	SaveObj("right.obj",dome_options.cuberight,dome_options.ncuberight);
   	SaveObj("top.obj",dome_options.cubetop,dome_options.ncubetop);
   	SaveObj("bottom.obj",dome_options.cubebottom,dome_options.ncubebottom);
	#endif

   return(TRUE);
}

void SaveObj(char *fname,DOMEFACE *f,int nf) 
{
	int i,j,k;
	int index[3];
	int found;
	double u=0,v=0;
	FILE *fptr;
	int np = 0;
	XYZ *p = NULL;

	fptr = fopen(fname,"w");
	fprintf(fptr,"mtllib none.mtl\n");

	// Find all unique vertices
	for (i=0;i<nf;i++) {
		for (k=0;k<3;k++) {
			found = FALSE;
			for (j=0;j<np;j++) {
				if (VectorEqual(f[i].p[k],p[j])) {
					found = TRUE;
					break;
				}
			}
			if (!found) {
				p = realloc(p,(np+1)*sizeof(XYZ));
				p[np] = f[i].p[k];
				np++;
			}
		}
	}
	fprintf(stderr,"Found %d unique vertices\n",np);

	// Write unique vertices
	for (i=0;i<np;i++) {
		fprintf(fptr,"v %lf %lf %lf\n",p[i].x,p[i].y,p[i].z);
	}

	// Write texture coordinates
	for (j=0;j<np;j++) {
		found = FALSE;
   	for (i=0;i<nf;i++) { 
      	for (k=0;k<3;k++) {
            if (VectorEqual(f[i].p[k],p[j])) {
               u = f[i].u[k];
               v = f[i].v[k];
					found = TRUE;
               break;
            }
         }  
			if (found)
				break;
		}
		if (!found)
			fprintf(stderr,"Failed to find texture coordinate - should not happen\n");
     	fprintf(fptr,"vt %lf %lf 0\n",u,v);
   }

	// Normals
   for (i=0;i<np;i++) {
      fprintf(fptr,"vn 0 1 0\n");
   }

	// Faces
	fprintf(fptr,"g 0\n");
	fprintf(fptr,"usemtl MTL0\n");
   for (i=0;i<nf;i++) {
		index[0] = -1;
		index[1] = -1;
		index[2] = -1;
      for (k=0;k<3;k++) {
			for (j=0;j<np;j++) {
				if (VectorEqual(f[i].p[k],p[j])) {
					index[k] = j;
					break;
				}
			}
		}
		if (index[0] < 0 || index[1] < 0 || index[2] < 0)
			fprintf(stderr,"Failed to find face index - should not happen\n");
      fprintf(fptr,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",
			index[0]+1,index[0]+1,index[0]+1,
         index[1]+1,index[1]+1,index[1]+1,
         index[2]+1,index[2]+1,index[2]+1);
   }

	free(p);
	fclose(fptr);
}

/*
   Flatten a dome to a planar object
   It is the planar object that is finally rendered in orthographic mode
*/
void Dome_CubeFlatten(XYZ *f,double aperture)
{
   double phi,r;

   r   = atan2(sqrt(f->x*f->x + f->z*f->z),f->y) / (DTOR * aperture / 2);
   phi = atan2(f->z,f->x);

   f->x = r * cos(phi);
   f->y = 0;
   f->z = r * sin(phi);
}


/*
   Refine a triangular mesh by bisecting each edge
   Forms 3 new triangles for each existing triangle on each iteration
   Could be made more efficient for drawing if the triangles were
   ordered in a fan or strip!
*/
void Dome_CubeSplitFace(DOMEFACE *f,int *n)
{
   int i;
   int n1,n2;

   n1 = *n;
   n2 = *n;

   for (i=0;i<n1;i++) {

      f[n2].p[0] = MidPoint(f[i].p[0],f[i].p[1]);
      f[n2].p[1] = f[i].p[1];
      f[n2].p[2] = MidPoint(f[i].p[1],f[i].p[2]);
      f[n2].u[0] = LinearInterpolate(f[i].u[0],f[i].u[1],0.5);
      f[n2].u[1] = f[i].u[1];
      f[n2].u[2] = LinearInterpolate(f[i].u[1],f[i].u[2],0.5);
      f[n2].v[0] = LinearInterpolate(f[i].v[0],f[i].v[1],0.5);
      f[n2].v[1] = f[i].v[1];
      f[n2].v[2] = LinearInterpolate(f[i].v[1],f[i].v[2],0.5);

      f[n2+1].p[0] = MidPoint(f[i].p[1],f[i].p[2]);
      f[n2+1].p[1] = f[i].p[2];
      f[n2+1].p[2] = MidPoint(f[i].p[2],f[i].p[0]);
      f[n2+1].u[0] = LinearInterpolate(f[i].u[1],f[i].u[2],0.5);
      f[n2+1].u[1] = f[i].u[2];
      f[n2+1].u[2] = LinearInterpolate(f[i].u[2],f[i].u[0],0.5);
      f[n2+1].v[0] = LinearInterpolate(f[i].v[1],f[i].v[2],0.5);
      f[n2+1].v[1] = f[i].v[2];
      f[n2+1].v[2] = LinearInterpolate(f[i].v[2],f[i].v[0],0.5);

      f[n2+2].p[0] = MidPoint(f[i].p[0],f[i].p[1]);
      f[n2+2].p[1] = MidPoint(f[i].p[1],f[i].p[2]);
      f[n2+2].p[2] = MidPoint(f[i].p[2],f[i].p[0]);
      f[n2+2].u[0] = LinearInterpolate(f[i].u[0],f[i].u[1],0.5);
      f[n2+2].u[1] = LinearInterpolate(f[i].u[1],f[i].u[2],0.5);
      f[n2+2].u[2] = LinearInterpolate(f[i].u[2],f[i].u[0],0.5);
      f[n2+2].v[0] = LinearInterpolate(f[i].v[0],f[i].v[1],0.5);
      f[n2+2].v[1] = LinearInterpolate(f[i].v[1],f[i].v[2],0.5);
      f[n2+2].v[2] = LinearInterpolate(f[i].v[2],f[i].v[0],0.5);

      //f[i].p[0] = f[i].p[0];
      f[i].p[1] = MidPoint(f[i].p[0],f[i].p[1]);
      f[i].p[2] = MidPoint(f[i].p[0],f[i].p[2]);
      //f[i].u[0] = f[i].u[0];
      f[i].u[1] = LinearInterpolate(f[i].u[0],f[i].u[1],0.5);
      f[i].u[2] = LinearInterpolate(f[i].u[0],f[i].u[2],0.5);
      //f[i].v[0] = f[i].v[0];
      f[i].v[1] = LinearInterpolate(f[i].v[0],f[i].v[1],0.5);
      f[i].v[2] = LinearInterpolate(f[i].v[0],f[i].v[2],0.5);

      n2 += 3;
   }

   *n = n2;
}

/*
   Draw the fisheye from the cubic maps
*/
void Dome_FishDraw(int refresh)
{
   int i,j;
   static GLuint displaylist[4] = {-1,-1,-1,-1};

   // Colour and normals
   glColor3f(1.0,1.0,1.0);
   glNormal3f(0.0,1.0,0.0);

	// Free existing displaylist ids
	if (refresh) {
		for (i=0;i<4;i++) {
			if (displaylist[i] > 0)
				glDeleteLists(displaylist[i],1);
			displaylist[i] = glGenLists(1);
		}
	}

	glEnable(dome_options.texturetype);

   /* Top face */
   glBindTexture(dome_options.texturetype,dome_options.textureid[0]);
	if (!refresh) {
		glCallList(displaylist[0]);
	} else {
	   glNewList(displaylist[0],GL_COMPILE);
   	glBegin(GL_TRIANGLES);
   	for (i=0;i<dome_options.ncubetop;i++) {
   	   for (j=0;j<3;j++) {
   	      Dome_NodeUV(dome_options.cubetop[i].u[j],dome_options.cubetop[i].v[j],
					dome_options.cubesize,dome_options.cubesize);
         	Dome_NodeXY(dome_options.cubetop[i].p[j]);
			}
      }
   	glEnd();
   	glEndList();
	}

   /* Bottom face */
   glBindTexture(dome_options.texturetype,dome_options.textureid[1]);
   if (!refresh) {
      glCallList(displaylist[1]);
   } else {
   	glNewList(displaylist[1],GL_COMPILE);
   	glBegin(GL_TRIANGLES);
   	for (i=0;i<dome_options.ncubebottom;i++) {
      	for (j=0;j<3;j++) {
         	Dome_NodeUV(dome_options.cubebottom[i].u[j],dome_options.cubebottom[i].v[j],
					dome_options.cubesize,dome_options.cubesize);
         	Dome_NodeXY(dome_options.cubebottom[i].p[j]);
			}
      }
   	glEnd();
   	glEndList();
	}

   /* Left face */
   glBindTexture(dome_options.texturetype,dome_options.textureid[2]);
   if (!refresh) {
      glCallList(displaylist[2]);
   } else {
   	glNewList(displaylist[2],GL_COMPILE);
   	glBegin(GL_TRIANGLES);
   	for (i=0;i<dome_options.ncubeleft;i++) {
      	for (j=0;j<3;j++) {
         	Dome_NodeUV(dome_options.cubeleft[i].u[j],dome_options.cubeleft[i].v[j],
					dome_options.cubesize,dome_options.cubesize);
         	Dome_NodeXY(dome_options.cubeleft[i].p[j]);
      	}
   	}
   	glEnd();
   	glEndList();
	}	

   /* Right face */
   glBindTexture(dome_options.texturetype,dome_options.textureid[3]);
   if (!refresh) {
      glCallList(displaylist[3]);
   } else {
   	glNewList(displaylist[3],GL_COMPILE);
   	glBegin(GL_TRIANGLES);
   	for (i=0;i<dome_options.ncuberight;i++) {
      	for (j=0;j<3;j++) {
         	Dome_NodeUV(dome_options.cuberight[i].u[j],dome_options.cuberight[i].v[j],
					dome_options.cubesize,dome_options.cubesize);
         	Dome_NodeXY(dome_options.cuberight[i].p[j]);
      	}
   	}
   	glEnd();
   	glEndList();
	}

   glDisable(dome_options.texturetype);

	// Do we also draw the mesh
   if (dome_options.showcubemesh) {
		glColor3f(0.75,0.75,0.75);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glCallList(displaylist[0]);
		glCallList(displaylist[1]);
		glCallList(displaylist[2]);
		glCallList(displaylist[3]);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(1.0,1.0,1.0);
   }

}

/*
	Create the cubic texture images
	Only called once!
*/
int Dome_CubeTexture(void)
{
	int i;
   //BITMAP4 red = {255,0,0,255},blue = {0,0,255,255};
   //BITMAP4 green = {0,255,0,255}, magenta = {255,0,255,255};

	// Free memory if already malloced
   if (dome_options.texturetop != NULL)
      free(dome_options.texturetop);
   if (dome_options.texturebottom != NULL)
      free(dome_options.texturebottom);
   if (dome_options.textureleft != NULL)
      free(dome_options.textureleft);
   if (dome_options.textureright != NULL)
      free(dome_options.textureright);

	// Malloc new storage
   dome_options.texturetop = Create_Bitmap(dome_options.cubesize,dome_options.cubesize);
   dome_options.texturebottom = Create_Bitmap(dome_options.cubesize,dome_options.cubesize);
   dome_options.textureleft = Create_Bitmap(dome_options.cubesize,dome_options.cubesize);
   dome_options.textureright = Create_Bitmap(dome_options.cubesize,dome_options.cubesize);

	// Did it all go well?
   if (dome_options.texturetop == NULL || dome_options.texturebottom == NULL || 
		 dome_options.textureleft == NULL || dome_options.textureright == NULL) {
		if (dome_options.debug)
      	fprintf(stderr,"Texture creation failed\n");
      return(FALSE);
   }

   /* Plain colours 
   Erase_Bitmap(dome_options.texturetop,   dome_options.cubesize,dome_options.cubesize,blue);
   Erase_Bitmap(dome_options.texturebottom,dome_options.cubesize,dome_options.cubesize,green);
   Erase_Bitmap(dome_options.textureleft,  dome_options.cubesize,dome_options.cubesize,red);
   Erase_Bitmap(dome_options.textureright, dome_options.cubesize,dome_options.cubesize,magenta);
	*/
	
   // Texture IDs for each face
   glGenTextures(4,dome_options.textureid);
   for (i=0;i<4;i++) {
      glBindTexture(dome_options.texturetype,dome_options.textureid[i]);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      switch (i) {
      case 0:
         glTexImage2D(dome_options.texturetype,0,GL_RGB,dome_options.cubesize,
				dome_options.cubesize,0,GL_RGBA,GL_UNSIGNED_BYTE,dome_options.texturetop);
         break;
      case 1:
         glTexImage2D(dome_options.texturetype,0,GL_RGB,dome_options.cubesize,
				dome_options.cubesize,0,GL_RGBA,GL_UNSIGNED_BYTE,dome_options.texturebottom);
         break;
      case 2:
         glTexImage2D(dome_options.texturetype,0,GL_RGB,dome_options.cubesize,
				dome_options.cubesize,0,GL_RGBA,GL_UNSIGNED_BYTE,dome_options.textureleft);
         break;
      case 3:
         glTexImage2D(dome_options.texturetype,0,GL_RGB,dome_options.cubesize,
				dome_options.cubesize,0,GL_RGBA,GL_UNSIGNED_BYTE,dome_options.textureright);
      }
   }

	return(TRUE);
}

/*
	Create the fisheye texture
	Deal with the possibility of using frame buffer objects
	Only performed once!
*/
int Dome_FishTexture(void)
{
#ifdef TRYFBO
   dome_options.fbostatus = TRUE;
   switch (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) {
   case GL_FRAMEBUFFER_COMPLETE_EXT:
      break;
   case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      dome_options.fbostatus = FALSE;
      break;
   default:
      dome_options.fbostatus = FALSE;
      break;
   }
   if (dome_options.fbostatus) {
      glGenFramebuffersEXT(1,&dome_options.fbo);
      dome_options.fishsize = (glutGet(GLUT_SCREEN_WIDTH) + glutGet(GLUT_SCREEN_HEIGHT)) / 2;
   	if (dome_options.debug)
	      fprintf(stderr,"Fisheye texture size: %d\n",dome_options.fishsize);
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,dome_options.fbo);
      glGenRenderbuffersEXT(1,&dome_options.depthbuffer);
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,dome_options.depthbuffer);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,dome_options.fishsize,dome_options.fishsize);
      glGenTextures(1,&dome_options.fishtextureid);
      glBindTexture(dome_options.texturetype,dome_options.fishtextureid);
      glTexImage2D(dome_options.texturetype,0,GL_RGBA8,dome_options.fishsize,
			dome_options.fishsize,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			dome_options.texturetype,dome_options.fishtextureid, 0);
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,dome_options.depthbuffer);
      if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
			if (dome_options.debug)
         	fprintf(stderr,"Frame buffer object creation failed\n");
         dome_options.fbostatus = FALSE;
      }
      if (dome_options.fbostatus)
         glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   }
#endif

   if (!dome_options.fbostatus) {
      dome_options.fishsize = dome_options.cubesize;
      dome_options.fishimage = Create_Bitmap(dome_options.fishsize,dome_options.fishsize);
      glGenTextures(1,&dome_options.fishtextureid);
      glBindTexture(dome_options.texturetype,dome_options.fishtextureid);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameterf(dome_options.texturetype,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexImage2D(dome_options.texturetype,0,GL_RGB,dome_options.cubesize,
			dome_options.cubesize,0,GL_RGBA,GL_UNSIGNED_BYTE,dome_options.fishimage);
   }

	return(TRUE);
}

/*
	This is the mian display function
	The result is a warped image on the back buffer ready for the calling application to do a swapbuffer
	Push and pop all OpenGL attributes
*/
void Dome_Display(XYZ vp,XYZ vd,XYZ vu,int width,int height,double near,double far)
{
   int i,j;
   XYZ vr,vl;
   XYZ vright,vleft;

   // Calculate various view vectors
	Normalise(&vd);
	Normalise(&vu);
   vr = CrossProduct(vd,vu);
   Normalise(&vr);
   vl = CrossProduct(vu,vd);
   Normalise(&vl);
   vright = VectorAdd(vr,vd);
   vleft = VectorAdd(vl,vd);

   /*
      Draw each of the 4 views and make textures of the result
      For a general dome need left, right, top, bottom
      Could get away with three views for the two truncated domes - too messy
   */
   glClearColor(0.0,0.0,0.0,0.0);
   glDrawBuffer(GL_BACK);
   glReadBuffer(GL_BACK);
   glViewport(0,0,dome_options.cubesize,dome_options.cubesize);

   // Left
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0,1.0,near,far);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(vp.x,vp.y,vp.z,vp.x+vleft.x,vp.y+vleft.y,vp.z+vleft.z,vu.x,vu.y,vu.z);
   dome_options.drawfcn(FALSE);
   glBindTexture(dome_options.texturetype,dome_options.textureid[2]);
   glCopyTexSubImage2D(dome_options.texturetype,0,0,0,0,0,dome_options.cubesize,dome_options.cubesize);

   // Right
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0,1.0,near,far);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(vp.x,vp.y,vp.z,vp.x+vright.x,vp.y+vright.y,vp.z+vright.z,vu.x,vu.y,vu.z);
   dome_options.drawfcn(FALSE);
   glBindTexture(dome_options.texturetype,dome_options.textureid[3]);
   glCopyTexSubImage2D(dome_options.texturetype,0,0,0,0,0,dome_options.cubesize,dome_options.cubesize);

   // Top
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0,1.0,near,far);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(vp.x,vp.y,vp.z,vp.x+vu.x,vp.y+vu.y,vp.z+vu.z,-vright.x,-vright.y,-vright.z);
   dome_options.drawfcn(FALSE);
   glBindTexture(dome_options.texturetype,dome_options.textureid[0]);
   glCopyTexSubImage2D(dome_options.texturetype,0,0,0,0,0,dome_options.cubesize,dome_options.cubesize);

   // Bottom
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0,1.0,near,far);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(vp.x,vp.y,vp.z,vp.x-vu.x,vp.y-vu.y,vp.z-vu.z,-vleft.x,-vleft.y,-vleft.z);
   dome_options.drawfcn(FALSE);
   glBindTexture(dome_options.texturetype,dome_options.textureid[1]);
   glCopyTexSubImage2D(dome_options.texturetype,0,0,0,0,0,dome_options.cubesize,dome_options.cubesize);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	Dome_OpenglState();

   /*
      Draw the fisheye, aspect ratio = 1
      Capture the fisheye as a texture
   */
   if (!dome_options.fbostatus) {
      glDrawBuffer(GL_BACK);
#ifdef TRYFBO
   } else {
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,dome_options.fbo);
#endif
   }
   glClearColor(0.0,0.0,0.0,0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0,0,dome_options.fishsize,dome_options.fishsize);
   glOrtho(-1.0,1.0,  -1.0,1.0,  0.1,10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0,-1.0,0.0,   0.0,0.0,0.0,   0.0,0.0,1.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   Dome_FishDraw(FALSE);
   if (!dome_options.fbostatus) {
      glBindTexture(dome_options.texturetype,dome_options.fishtextureid);
      glCopyTexSubImage2D(dome_options.texturetype,0,0,0,0,0,dome_options.fishsize,dome_options.fishsize);
#ifdef TRYFBO
   } else {
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
   }

	/*
      Finally draw the fisheye texture onto the warp mesh
   */
   glDrawBuffer(GL_BACK);
   glClearColor(0.0,0.0,0.0,0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0,0,width,height);
   glOrtho(-width/(double)height,width/(double)height,  -1.0,1.0,  0.1,10);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0,-1.0,0.0,   0.0,0.0,0.0,   0.0,0.0,1.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glColor3f(1.0,1.0,1.0);
   glNormal3f(0.0,0.0,-1.0);
   glEnable(dome_options.texturetype);
   glBindTexture(dome_options.texturetype,dome_options.fishtextureid);
   for (j=0;j<dome_options.meshny-1;j++) {
      glBegin(GL_TRIANGLE_STRIP);
      for (i=0;i<dome_options.meshnx;i++) {
			if (dome_options.showintensity)
         	glColor3f(dome_options.mesh[i][j].i,dome_options.mesh[i][j].i,dome_options.mesh[i][j].i);
         Dome_NodeUV(dome_options.mesh[i][j].u,dome_options.mesh[i][j].v,dome_options.fishsize,dome_options.fishsize);
         glVertex3f(dome_options.mesh[i][j].x,0.0,dome_options.mesh[i][j].y);
			if (dome_options.showintensity)
         	glColor3f(dome_options.mesh[i][j+1].i,dome_options.mesh[i][j+1].i,dome_options.mesh[i][j+1].i);
         Dome_NodeUV(dome_options.mesh[i][j+1].u,dome_options.mesh[i][j+1].v,dome_options.fishsize,dome_options.fishsize);
         glVertex3f(dome_options.mesh[i][j+1].x,0.0,dome_options.mesh[i][j+1].y);
      }
      glEnd();
   }
   glDisable(dome_options.texturetype);

	// Optionally draw the mesh itself
   if (dome_options.showfishmesh) {
      glColor3f(0.75,0.75,0.75);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glBegin(GL_QUADS);
      for (j=0;j<dome_options.meshny-1;j++) {
         for (i=0;i<dome_options.meshnx-1;i++) {
            glVertex3f(dome_options.mesh[i][j].x,0.0,dome_options.mesh[i][j].y);
            glVertex3f(dome_options.mesh[i][j+1].x,0.0,dome_options.mesh[i][j+1].y);
            glVertex3f(dome_options.mesh[i+1][j+1].x,0.0,dome_options.mesh[i+1][j+1].y);
            glVertex3f(dome_options.mesh[i+1][j].x,0.0,dome_options.mesh[i+1][j].y);
         }
      }
      glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(1.0,1.0,1.0);
   }
	glPopAttrib();

}

/*
   Basic drawing state
*/
void Dome_OpenglState(void)
{
   glDisable(GL_LIGHTING);
   //glDisable(GL_ALPHA_TEST);
   //glDisable(GL_COLOR_MATERIAL);
   //glDisable(GL_DITHER);
   //glDisable(GL_FOG);
   //glDisable(GL_LINE_SMOOTH);
   //glDisable(GL_POINT_SMOOTH);
   //glDisable(GL_POLYGON_SMOOTH);
   //glDisable(GL_LINE_STIPPLE);
   //glDisable(GL_SCISSOR_TEST);
   //glDisable(GL_STENCIL_TEST);
   //glDisable(GL_CULL_FACE);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   //glEnable(GL_COLOR_MATERIAL);
   //glPixelStorei(GL_UNPACK_ALIGNMENT,1);
   //glLineWidth(1.0);
   //glPointSize(1.0);
   glFrontFace(GL_CW);
   //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
   //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
   glClearColor(0.0,0.0,0.0,0.0);
   glDisable(GL_DEPTH_TEST);
}

