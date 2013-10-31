#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "paulslib.h"
#include "povraylib.h"

/*
	Define a line in PovRay
*/
void WritePovLine(FILE *fptr,XYZ p1,XYZ p2,double r,char *texture,int ends)
{
   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
		p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,r);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
	
	if (ends == 1 || ends == 3) {
   	fprintf(fptr,"sphere {\n");
   	fprintf(fptr,"   <%g,%g,%g>, %g\n",p1.x,p1.y,p1.z,r);
   	fprintf(fptr,"   texture { %s }\n",texture);
   	fprintf(fptr,"}\n");
	}
	if (ends == 2 || ends == 3) {
      fprintf(fptr,"sphere {\n");
      fprintf(fptr,"   <%g,%g,%g>, %g\n",p2.x,p2.y,p2.z,r);
      fprintf(fptr,"   texture { %s }\n",texture);
      fprintf(fptr,"}\n");
	}
}

/*
	Create a PovRay bounding box
*/
void WritePovBox(FILE *fptr,XYZ p1,XYZ p2,double radius,char *texture)
{
	/* Cylinders */

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p1.x,p1.y,p1.z,p2.x,p1.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p1.x,p1.y,p1.z,p1.x,p2.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p1.x,p1.y,p1.z,p1.x,p1.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p2.x,p2.y,p2.z,p1.x,p2.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p2.x,p2.y,p2.z,p2.x,p1.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p2.x,p2.y,p2.z,p2.x,p2.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p1.x,p2.y,p1.z,p2.x,p2.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p1.x,p2.y,p1.z,p1.x,p2.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p2.x,p1.y,p1.z,p2.x,p1.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p2.x,p1.y,p1.z,p2.x,p2.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p1.x,p1.y,p2.z,p1.x,p2.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

   fprintf(fptr,"cylinder {\n");
   fprintf(fptr,"   <%g,%g,%g>, <%g,%g,%g>, %g\n",
      p1.x,p1.y,p2.z,p2.x,p1.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");

	/* Spheres */

   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p1.x,p1.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p2.x,p1.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p2.x,p2.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p1.x,p2.y,p1.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p1.x,p1.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p2.x,p1.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p2.x,p2.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
   fprintf(fptr,"sphere {\n");
   fprintf(fptr,"   <%g,%g,%g>, %g\n",p1.x,p2.y,p2.z,radius);
   fprintf(fptr,"   texture { %s }\n",texture);
   fprintf(fptr,"}\n");
}

