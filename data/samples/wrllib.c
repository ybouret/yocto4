#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "paulslib.h"
#include "wrllib.h"

void WRLLine(FILE *fptr,XYZ *p,int n,COLOUR c)
{
   int i;

   fprintf(fptr,"Shape {\n");
   fprintf(fptr," geometry IndexedLineSet {\n");
   fprintf(fptr,"  coord  Coordinate {\n");
   fprintf(fptr,"   point [\n");
   for (i=0;i<n;i++)
      fprintf(fptr,"    %g %g %g%c\n",p[i].x,p[i].y,p[i].z,i<n-1?',':' ');
   fprintf(fptr,"   ]\n");
   fprintf(fptr,"  }\n");
   fprintf(fptr,"  color Color {\n");
   fprintf(fptr,"   color [\n");
   for (i=0;i<n;i++)
      fprintf(fptr,"    %g %g %g%c\n",c.r,c.g,c.b,i<n-1?',':' ');
   fprintf(fptr,"   ]\n");
   fprintf(fptr,"  }\n");
   fprintf(fptr,"  coordIndex [ ");
   for (i=0;i<n;i++) {
      fprintf(fptr,"    %d,",i);
      if ((i+1) % 10 == 0)
         fprintf(fptr,"\n");
   }
   fprintf(fptr,"  -1\n");
   fprintf(fptr,"  ]\n");
   fprintf(fptr," }\n");
   fprintf(fptr,"}\n");
}

void WRLLine2(FILE *fptr,XYZ p1,XYZ p2,COLOUR c)
{
   fprintf(fptr,"Shape {\n");
   fprintf(fptr," geometry IndexedLineSet {\n");
   fprintf(fptr,"  coord  Coordinate {\n");
   fprintf(fptr,"   point [\n");
   fprintf(fptr,"    %g %g %g,\n",p1.x,p1.y,p1.z);
   fprintf(fptr,"    %g %g %g\n",p2.x,p2.y,p2.z);
   fprintf(fptr,"   ]\n");
   fprintf(fptr,"  }\n");
   fprintf(fptr,"  color Color {\n");
   fprintf(fptr,"   color [\n");
   fprintf(fptr,"    %g %g %g,\n",c.r,c.g,c.b);
   fprintf(fptr,"    %g %g %g\n",c.r,c.g,c.b);
   fprintf(fptr,"   ]\n");
   fprintf(fptr,"  }\n");
   fprintf(fptr,"  coordIndex [ 0,1,-1 ]\n");
   fprintf(fptr," }\n");
   fprintf(fptr,"}\n");
}

/*
   WRL header
   Assume the object is at the origin
*/
void WRLHeader(FILE *fptr,char *comment,double dist)
{
   fprintf(fptr,"#VRML V2.0 utf8\n");

   fprintf(fptr,"WorldInfo {\n");
   fprintf(fptr," title \"%s\"\n",comment);
   fprintf(fptr," info [ \"By Paul Bourke\" ]\n");
   fprintf(fptr,"}\n");

   fprintf(fptr,"NavigationInfo {\n");
   fprintf(fptr," type [\"EXAMINE\",\"ANY\"]\n");
   fprintf(fptr," headlight TRUE\n");
   fprintf(fptr," visibilityLimit 0.0\n");
   fprintf(fptr," speed 1.0\n");
   fprintf(fptr,"}\n");

   fprintf(fptr,"DEF DefaultView Viewpoint {\n");
   fprintf(fptr," position 0 0 %g\n",dist);
   fprintf(fptr," description \"Default view\"\n");
   fprintf(fptr," fieldOfView %g\n",PI/4);
   fprintf(fptr,"}\n");
}

void WRLSphere(FILE *fptr,XYZ p,double r,COLOUR c)
{
   fprintf(fptr,"Transform {\n");
   fprintf(fptr," translation %g,%g,%g\n",p.x,p.y,p.z);
   fprintf(fptr," children [\n");
   fprintf(fptr,"  Shape {\n");
   fprintf(fptr,"   appearance Appearance {\n");
   fprintf(fptr,"    material Material {\n");
   fprintf(fptr,"     diffuseColor %g %g %g\n",c.r,c.g,c.b);
   fprintf(fptr,"    }\n");
   fprintf(fptr,"   }\n");
   fprintf(fptr,"   geometry Sphere {\n");
   fprintf(fptr,"    radius %g\n",r);
   fprintf(fptr,"   }\n");
   fprintf(fptr,"  }\n");
   fprintf(fptr," ]\n");
   fprintf(fptr,"}\n");
}

void WRLSphere2(FILE *fptr,XYZ c,double r,int n,COLOUR col)
{
   int i,j;
   double theta1,theta2,theta3;
   XYZ e,p[4];

   if (r < 0)
      r = -r;
   if (n < 0)
      n = -n;
   if (n < 4 || r <= 0)
      n = 4;

   /* Make n even */
   n /= 2;
   n *= 2;

   for (j=0;j<n/2;j++) {
      theta1 = j * TWOPI / n - PID2;
      theta2 = (j + 1) * TWOPI / n - PID2;

      for (i=0;i<n;i++) {
         theta3 = i * TWOPI / n;

         e.x = cos(theta2) * cos(theta3);
         e.y = sin(theta2);
         e.z = cos(theta2) * sin(theta3);
         p[0].x = c.x + r * e.x;
         p[0].y = c.y + r * e.y;
         p[0].z = c.z + r * e.z;

         e.x = cos(theta1) * cos(theta3);
         e.y = sin(theta1);
         e.z = cos(theta1) * sin(theta3);
         p[1].x = c.x + r * e.x;
         p[1].y = c.y + r * e.y;
         p[1].z = c.z + r * e.z;

         theta3 = (i+1) * TWOPI / n;

         e.x = cos(theta2) * cos(theta3);
         e.y = sin(theta2);
         e.z = cos(theta2) * sin(theta3);
         p[3].x = c.x + r * e.x;
         p[3].y = c.y + r * e.y;
         p[3].z = c.z + r * e.z;

         e.x = cos(theta1) * cos(theta3);
         e.y = sin(theta1);
         e.z = cos(theta1) * sin(theta3);
         p[2].x = c.x + r * e.x;
         p[2].y = c.y + r * e.y;
         p[2].z = c.z + r * e.z;

         WRLFace(fptr,p,4,col);
      }
   }
}

/*
   A cone, capped or uncapped
   From vertex p1 to p2, radii r1 and r2
   Resolution: m
   Calls WRLFace == rather inefficient
*/
void WRLCone(FILE *fptr,XYZ p1,XYZ p2,double r1,double r2,int m,COLOUR c,int caps)
{
   int i;
   double theta1 = 0,theta2 = TWOPI;
   XYZ n1,n,r[4],p[4],q,perp;

   /* Normal pointing from p1 to p2 */
   n1.x = p1.x - p2.x;
   n1.y = p1.y - p2.y;
   n1.z = p1.z - p2.z;
   Normalise(&n1);

   /*
      Create two perpendicular vectors perp and q
      on the plane of the disk
   */
   perp = n1;
   if (n1.x == 0 && n1.z == 0)
      perp.x += 1;
   else
      perp.y += 1;
   CROSSPROD(perp,n1,q);
   CROSSPROD(n1,q,perp);
   Normalise(&perp);
   Normalise(&q);

   for (i=0;i<m;i++) {
      theta1 = i * TWOPI / m;
      n.x = cos(theta1) * perp.x + sin(theta1) * q.x;
      n.y = cos(theta1) * perp.y + sin(theta1) * q.y;
      n.z = cos(theta1) * perp.z + sin(theta1) * q.z;
      Normalise(&n);

      p[0].x = p1.x + r1 * n.x;
      p[0].y = p1.y + r1 * n.y;
      p[0].z = p1.z + r1 * n.z;
      p[1].x = p2.x + r2 * n.x;
      p[1].y = p2.y + r2 * n.y;
      p[1].z = p2.z + r2 * n.z;

      theta2 = (i+1) * TWOPI / m;
      n.x = cos(theta2) * perp.x + sin(theta2) * q.x;
      n.y = cos(theta2) * perp.y + sin(theta2) * q.y;
      n.z = cos(theta2) * perp.z + sin(theta2) * q.z;
      Normalise(&n);

      p[3].x = p1.x + r1 * n.x;
      p[3].y = p1.y + r1 * n.y;
      p[3].z = p1.z + r1 * n.z;
      p[2].x = p2.x + r2 * n.x;
      p[2].y = p2.y + r2 * n.y;
      p[2].z = p2.z + r2 * n.z;

      WRLFace(fptr,p,4,c);

      if (caps) {
         r[0] = p1;
         r[1] = p[0];
         r[2] = p[3];
         WRLFace(fptr,r,3,c);
         r[0] = p2;
         r[1] = p[2];
         r[2] = p[1];
         WRLFace(fptr,r,3,c);
      }
   }
}

void WRLCone2(FILE *fptr,XYZ p1,XYZ p2,double r1,double r2,int m,COLOUR c,int caps)
{
   int i;
   double theta1 = 0;
   XYZ n1,n,p,q,perp;

   /* Normal pointing from p1 to p2 */
   n1.x = p1.x - p2.x;
   n1.y = p1.y - p2.y;
   n1.z = p1.z - p2.z;
   Normalise(&n1);

   /*
      Create two perpendicular vectors perp and q
      on the plane of the disk
   */
   perp = n1;
   if (n1.x == 0 && n1.z == 0)
      perp.x += 1;
   else
      perp.y += 1;
   CROSSPROD(perp,n1,q);
   CROSSPROD(n1,q,perp);
   Normalise(&perp);
   Normalise(&q);

   fprintf(fptr,"Shape {\n");
   fprintf(fptr," appearance Appearance {\n");
   fprintf(fptr,"  material Material {\n");
   fprintf(fptr,"   diffuseColor %g %g %g\n",c.r,c.g,c.b);
   fprintf(fptr,"  }\n");
   fprintf(fptr," }\n");
   fprintf(fptr," geometry IndexedFaceSet {\n");
   fprintf(fptr,"  coord  Coordinate {\n");
   fprintf(fptr,"   point [\n");

   for (i=0;i<m;i++) {
      theta1 = i * TWOPI / m;
      n.x = cos(theta1) * perp.x + sin(theta1) * q.x;
      n.y = cos(theta1) * perp.y + sin(theta1) * q.y;
      n.z = cos(theta1) * perp.z + sin(theta1) * q.z;
      Normalise(&n);

      p.x = p1.x + r1 * n.x;
      p.y = p1.y + r1 * n.y;
      p.z = p1.z + r1 * n.z;
		fprintf(fptr,"    %g %g %g,\n",p.x,p.y,p.z);
      p.x = p2.x + r2 * n.x;
      p.y = p2.y + r2 * n.y;
      p.z = p2.z + r2 * n.z;
		fprintf(fptr,"    %g %g %g,\n",p.x,p.y,p.z);
	}
	fprintf(fptr,"    %g %g %g,\n",p1.x,p1.y,p1.z);
	fprintf(fptr,"    %g %g %g\n",p2.x,p2.y,p2.z);
   fprintf(fptr,"   ]\n");
   fprintf(fptr,"  }\n");

   fprintf(fptr,"  coordIndex [\n");
   for (i=0;i<m;i++) {
		if (caps) {
			fprintf(fptr,"    %d,%d,%d,-1,",(2*i+1)%(2*m),m+1,(2*i+3)%(2*m));
      	fprintf(fptr,"    %d,%d,%d,-1,",2*i,m,(2*i+2)%(2*m));
		}
      fprintf(fptr,"    %d,%d,%d,%d,-1%c\n",
			2*i,(2*i+1)%(2*m),(2*i+3)%(2*m),(2*i+2)%(2*m),i<m-1?',':' ');
	}
   fprintf(fptr,"  ]\n");
   fprintf(fptr,"  solid FALSE\n");
   fprintf(fptr," }\n");
   fprintf(fptr,"}\n");
}

/*
   A face with np vertices and a normal per vertex
*/
void WRLFace(FILE *fptr,XYZ *p,int np,COLOUR c)
{
   int i;

   fprintf(fptr,"Shape {\n");
   fprintf(fptr," appearance Appearance {\n");
   fprintf(fptr,"  material Material {\n");
   fprintf(fptr,"   diffuseColor %g %g %g\n",c.r,c.g,c.b);
   fprintf(fptr,"  }\n");
   fprintf(fptr," }\n");
   fprintf(fptr," geometry IndexedFaceSet {\n");
   fprintf(fptr,"  coord  Coordinate {\n");
   fprintf(fptr,"   point [\n");
   for (i=0;i<np;i++)
      fprintf(fptr,"    %g %g %g%c\n",p[i].x,p[i].y,p[i].z,i<np-1?',':' ');
   fprintf(fptr,"   ]\n");
   fprintf(fptr,"  }\n");
   fprintf(fptr,"  coordIndex [\n");
   for (i=0;i<np;i++)
      fprintf(fptr,"    %d,",i);
   fprintf(fptr,"  -1\n");
   fprintf(fptr,"  ]\n");
   fprintf(fptr,"  solid FALSE\n");
   fprintf(fptr," }\n");
   fprintf(fptr,"}\n");
}


