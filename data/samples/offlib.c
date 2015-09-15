#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include <sys/resource.h>
#include <time.h>
#include "paulslib.h"
#include "offlib.h"

/*
   Write a label for Geomview
	The simplex font was derived from the hershey font set
	The array contains the vectors for characters 23 to 126 inclusive
	The first element for each character is the number of vectors
	The second element for each character is the width (proportional fonts)
	The remaining numbers are coordinates, (0,0) is bottom left.
	-1,-1 indicates a pen rise
	The characters at at most 25 high.
*/
void WriteOffLabel(FILE *fptr,char *s,XYZ p,double size,int axis,COLOUR colour)
{
	int c;
	int i,j;
	XYZ p1,p2;

#include "simplex.h"

	size /= 25;
	for (i=0;i<strlen(s);i++) {
		c = s[i];
		if (c < 32 || c > 126)
			continue;
		c -= 32;

		for (j=1;j<simplex[c][0];j++) {
			if (axis == 'x' || axis == 'X') {
				p1.x = p.x + size * simplex[c][2*(j-1)+2];
				p1.y = p.y + size * simplex[c][2*(j-1)+3];
				p1.z = p.z;
         	p2.x = p.x + size * simplex[c][2*j+2];
         	p2.y = p.y + size * simplex[c][2*j+3];
         	p2.z = p.z;
			} else if (axis == 'y' || axis == 'Y') {
            p1.x = p.x + size * simplex[c][2*(j-1)+3];
            p1.y = p.y - size * simplex[c][2*(j-1)+2];
            p1.z = p.z;
            p2.x = p.x + size * simplex[c][2*j+3];
            p2.y = p.y - size * simplex[c][2*j+2];
            p2.z = p.z;
			} else {
            p1.z = p.z - size * simplex[c][2*(j-1)+2];
            p1.x = p.x + size * simplex[c][2*(j-1)+3];
            p1.y = p.y;
            p2.z = p.z - size * simplex[c][2*j+2];
            p2.x = p.x + size * simplex[c][2*j+3];
            p2.y = p.y;
			}
         if ((simplex[c][2*(j-1)+2] == -1 && simplex[c][2*(j-1)+3] == -1) ||
             (simplex[c][2*j+2]     == -1 && simplex[c][2*j+3]     == -1))
            ;
         else
				WriteOffVector(fptr,p1,p2,colour);
		}

		if (axis == 'x' || axis == 'X')
			p.x += size * simplex[c][1];
		else if (axis == 'y' || axis == 'Y')
			p.y -= size * simplex[c][1];
		else
			p.z -= size * simplex[c][1];
	}
}

/*
	Write a facet in OFF format
*/
void WriteOffFacet(FILE *fptr,XYZ *p,int n,COLOUR colour)
{
	int i;

   fprintf(fptr,"{ = OFF\n");
   fprintf(fptr,"\t%d 1 1\n",n);
   for (i=0;i<n;i++)
      fprintf(fptr,"\t%g %g %g\n",p[i].x,p[i].y,p[i].z);
   fprintf(fptr,"\t%d ",n);
   for (i=0;i<n;i++)
      fprintf(fptr,"%d ",i);
   fprintf(fptr,"%g %g %g 1\n",colour.r,colour.g,colour.b);
   fprintf(fptr,"}\n");
}

/*
   Write a vector in OFF format
	Do not use for points
*/
void WriteOffVector(FILE *fptr,XYZ p1,XYZ p2,COLOUR colour)
{
   fprintf(fptr,"{ = VECT\n");
   fprintf(fptr,"\t1 2 1 2 1\n");
   fprintf(fptr,"\t%g %g %g\n",p1.x,p1.y,p1.z);
   fprintf(fptr,"\t%g %g %g\n",p2.x,p2.y,p2.z);
   fprintf(fptr,"\t%g %g %g 1\n",colour.r,colour.g,colour.b);
   fprintf(fptr,"}\n");
}

/*
   Write a curve in OFF format
	Do not use for points
*/
void WriteOffCurve(FILE *fptr,XYZ *p,int n,COLOUR colour)
{
   int i;

	fprintf(fptr,"{ = SKEL\n");
	fprintf(fptr,"%d 1\n",n);
	for (i=0;i<n;i++)
		fprintf(fptr,"%g %g %g\n",p[i].x,p[i].y,p[i].z);
	fprintf(fptr,"%d ",n);
	for (i=0;i<n;i++)
		fprintf(fptr,"%d ",i);
	fprintf(fptr,"%g %g %g 1\n",colour.r,colour.g,colour.b);
	fprintf(fptr,"}\n");
}

/*
   Write points in OFF format
	All the points are the same colour 
*/
void WriteOffPoints(FILE *fptr,XYZ *p,int n,COLOUR colour)
{
	int i;

   fprintf(fptr,"{ = VECT\n");
   fprintf(fptr,"%d %d %d\n",n,n,n);
	for (i=0;i<n;i++)                 /* Vertices in each polyline */
		fprintf(fptr,"1 ");
	fprintf(fptr,"\n");
   for (i=0;i<n;i++)                 /* Colour of each polyline */
      fprintf(fptr,"1 ");
   fprintf(fptr,"\n");
	for (i=0;i<n;i++)
   	fprintf(fptr,"%g %g %g\n",p[i].x,p[i].y,p[i].z);
	for (i=0;i<n;i++)
   	fprintf(fptr,"%g %g %g 1\n",colour.r,colour.g,colour.b);
   fprintf(fptr,"}\n");
}

/*
   Write a symbol in OFF format
	Types are: 0 - no marker
              1 - Plus sign
              2 - Point
              3 - Cube
*/
void WriteOffSymbol(FILE *fptr,XYZ p,COLOUR colour,double size,int type)
{
	switch (type) {
	case 0:											/* Invisible marker */
		break;
	case 1:											/* Plus sign */
   	fprintf(fptr,"{ = SKEL 6 3\n");
   	fprintf(fptr,"%g %g %g %g %g %g\n",
      	p.x-size/2,p.y,p.z,p.x+size/2,p.y,p.z);
   	fprintf(fptr,"%g %g %g %g %g %g\n",
      	p.x,p.y-size/2,p.z,p.x,p.y+size/2,p.z);
   	fprintf(fptr,"%g %g %g %g %g %g\n",
      	p.x,p.y,p.z-size/2,p.x,p.y,p.z+size/2);
   	fprintf(fptr,"2 0 1 %g %g %g 1\n",colour.r,colour.g,colour.b);
   	fprintf(fptr,"2 2 3 %g %g %g 1\n",colour.r,colour.g,colour.b);
   	fprintf(fptr,"2 4 5 %g %g %g 1\n",colour.r,colour.g,colour.b);
   	fprintf(fptr,"}\n");
		break;
	case 2:											/* Point, size not used */
		WriteOffPoints(fptr,&p,1,colour);
		break;
	case 3:
		fprintf(fptr,"{ = OFF\n");
		fprintf(fptr,"%d %d 1\n",8,6);
		fprintf(fptr,"%g %g %g\n",p.x+size/2,p.y-size/2,p.z+size/2);
      fprintf(fptr,"%g %g %g\n",p.x+size/2,p.y+size/2,p.z+size/2);
      fprintf(fptr,"%g %g %g\n",p.x-size/2,p.y+size/2,p.z+size/2);
      fprintf(fptr,"%g %g %g\n",p.x-size/2,p.y-size/2,p.z+size/2);
      fprintf(fptr,"%g %g %g\n",p.x+size/2,p.y-size/2,p.z-size/2);
      fprintf(fptr,"%g %g %g\n",p.x+size/2,p.y+size/2,p.z-size/2);
      fprintf(fptr,"%g %g %g\n",p.x-size/2,p.y+size/2,p.z-size/2);
      fprintf(fptr,"%g %g %g\n",p.x-size/2,p.y-size/2,p.z-size/2);
		fprintf(fptr,"4 0 1 2 3 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 4 5 6 7 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 1 2 6 5 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 0 4 7 3 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 2 3 7 6 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 0 1 5 4 %g %g %g 1\n",colour.r,colour.g,colour.b);
		fprintf(fptr,"}\n");
		break;
	}
}

/*
	Write a symbolic sphere in OOGL format
*/
void WriteOffSphere(fptr,c,r,colour)
FILE *fptr;
XYZ c;
double r;
COLOUR colour;
{
	int i,np=36;
	double theta;
	XYZ p[37],p1,p2;

	/* Draw the axes crosshairs */
	p1.x = c.x - r;
	p1.y = c.y;
	p1.z = c.z;
	p2.x = c.x + r;
	p2.y = c.y;
	p2.z = c.z;
	WriteOffVector(fptr,p1,p2,colour);
   p1.x = c.x;
   p1.y = c.y - r;
   p1.z = c.z;
   p2.x = c.x;
   p2.y = c.y + r;
   p2.z = c.z;
   WriteOffVector(fptr,p1,p2,colour);
   p1.x = c.x;
   p1.y = c.y;
   p1.z = c.z - r;
   p2.x = c.x;
   p2.y = c.y;
   p2.z = c.z + r;
   WriteOffVector(fptr,p1,p2,colour);

	/* Draw the axis circles */
	for (i=0;i<np;i++) {
		theta = i * TWOPI / np;
		p[i].x = c.x + r * sin(theta);
		p[i].y = c.y + r * cos(theta);
		p[i].z = c.z; 
	}
	p[np] = p[0];
	WriteOffCurve(fptr,p,np+1,colour);
   for (i=0;i<np;i++) {
      theta = i * TWOPI / np;
      p[i].x = c.x + r * sin(theta); 
		p[i].y = c.y;
      p[i].z = c.z + r * cos(theta);
   }
   p[np] = p[0];
   WriteOffCurve(fptr,p,np+1,colour);
   for (i=0;i<np;i++) {
      theta = i * TWOPI / np;
		p[i].x = c.x;
      p[i].y = c.y + r * sin(theta); 
      p[i].z = c.z + r * cos(theta);
   }
   p[np] = p[0];
   WriteOffCurve(fptr,p,np+1,colour);
}

/*
	Draw a box in off format
	type == 0 is a wireframe box
	        1 is a facet box
*/
void WriteOffBox(fptr,p1,p2,type,colour)
FILE *fptr;
XYZ p1,p2;
int type;
COLOUR colour;
{
	if (type == 0) {				/* Wire frame */
		fprintf(fptr,"{ = SKEL\n");
		fprintf(fptr,"8 4\n");
      fprintf(fptr,"%g %g %g\n",p1.x,p1.y,p1.z);
      fprintf(fptr,"%g %g %g\n",p2.x,p1.y,p1.z);
      fprintf(fptr,"%g %g %g\n",p2.x,p2.y,p1.z);
      fprintf(fptr,"%g %g %g\n",p1.x,p2.y,p1.z);
      fprintf(fptr,"%g %g %g\n",p1.x,p1.y,p2.z);
      fprintf(fptr,"%g %g %g\n",p2.x,p1.y,p2.z);
      fprintf(fptr,"%g %g %g\n",p2.x,p2.y,p2.z);
      fprintf(fptr,"%g %g %g\n",p1.x,p2.y,p2.z);
		fprintf(fptr,"9 0 1 2 6 5 4 7 3 0 4\n");
		fprintf(fptr,"2 6 7\n");
		fprintf(fptr,"2 1 5\n");
		fprintf(fptr,"2 2 3\n");
		fprintf(fptr,"}\n");
	} else if (type == 1) {		/* Facets */
		fprintf(fptr,"{ = OFF\n");
		fprintf(fptr,"8 6 1\n");
		fprintf(fptr,"%g %g %g\n",p1.x,p1.y,p1.z);
		fprintf(fptr,"%g %g %g\n",p2.x,p1.y,p1.z);
      fprintf(fptr,"%g %g %g\n",p2.x,p2.y,p1.z);
      fprintf(fptr,"%g %g %g\n",p1.x,p2.y,p1.z);
      fprintf(fptr,"%g %g %g\n",p1.x,p1.y,p2.z);
      fprintf(fptr,"%g %g %g\n",p2.x,p1.y,p2.z);
      fprintf(fptr,"%g %g %g\n",p2.x,p2.y,p2.z);
      fprintf(fptr,"%g %g %g\n",p1.x,p2.y,p2.z);
		fprintf(fptr,"4 0 1 2 3 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 0 4 5 1 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 0 3 7 4 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 2 6 7 3 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 1 5 6 2 %g %g %g 1\n",colour.r,colour.g,colour.b);
      fprintf(fptr,"4 4 7 6 5 %g %g %g 1\n",colour.r,colour.g,colour.b);
		fprintf(fptr,"}\n");
	}
}

/*
   Write a surface for Geomview
*/
int WriteOffSurface(fptr,tri,ntri,colour)
FILE *fptr;
TRIANGLE *tri;
int ntri;
COLOUR colour;
{
	int t,i,j,k=0,found;
	double dx,dy,dz;

	typedef struct {
		int node[3];
	} FACE;
	int nfacets = 0;
	FACE *facet;
	int nnodes = 0;
	XYZ *nodes;

	if ((nodes = malloc(ntri*3*sizeof(XYZ))) == NULL) 
		return(FALSE);
	if ((facet = malloc(ntri*sizeof(FACE))) == NULL)
		return(FALSE);

	for (t=0;t<ntri;t++) {
      for (j=0;j<3;j++) {
         found = FALSE;
         for (i=0;i<nnodes;i++) {
				dx = tri[t].p[j].x - nodes[i].x;
				dy = tri[t].p[j].y - nodes[i].y;
				dz = tri[t].p[j].z - nodes[i].z;
            if (sqrt(dx*dx + dy*dy + dz*dz) < EPS) {
               found = TRUE;
               k = i;
               break;
            }
         }
         if (!found) {
            k = nnodes;
            nodes[nnodes] = tri[t].p[j];
            nnodes++;
         }
         facet[nfacets].node[j] = k;
      }
      nfacets++;
	}

   printf("LIST\n");
   printf("{ = OFF\n");
   printf("%d %d 1\n",nnodes,nfacets);
   for (i=0;i<nnodes;i++)
      printf("%g %g %g\n",nodes[i].x,nodes[i].y,nodes[i].z);
   for (i=0;i<nfacets;i++)
      printf("3 %d %d %d %g %g %g 1\n",
         facet[i].node[0],facet[i].node[1],facet[i].node[2],
         colour.r,colour.g,colour.b);
   printf("}\n");

	return(TRUE);
}

/*
	Create a coordinate axis in OOGL
*/
void WriteOffAxis(
	FILE *fptr,
	char axis,			/* 'x' or 'y' or 'z' 	*/
	double start,		/* Starting position		*/
	double stop, 		/* Stopping position		*/
	double startl,		/* Starting label value	*/
	double stopl,		/* Stopping label value */
	int ticks,			/* Number of ticks		*/
	double ticklen,	/* Tick length				*/
	COLOUR colour)		/* Axis colour				*/
{
	XYZ p,q;
	int i;
	char s[32];

	switch (axis) {
	case 'x':
		p.x = start;
		p.y = 0;
		p.z = 0;
		q.x = stop;
		q.y = 0;
		q.z = 0;
		WriteOffVector(fptr,p,q,colour);	
		if (ticks <= 0)
			break;
		for (i=0;i<=ticks;i++) {
			p.x = start + i * (stop - start) / ticks;
			p.y = 0;
			p.z = 0;
			q.x = p.x;
			q.y = - ticklen;
			q.z = 0;
			WriteOffVector(fptr,p,q,colour);
			sprintf(s,"%g",startl + i * (stopl - startl) / ticks);
			p.y -= 2.0 * ticklen;
			p.x -= ticklen * strlen(s) / 2;
			WriteOffLabel(fptr,s,p,(double)ticklen,'x',colour);
		}
		break;
	case 'y':
      p.x = 0;   
      p.y = start;
      p.z = 0;
      q.x = 0;
      q.y = stop;
      q.z = 0;
      WriteOffVector(fptr,p,q,colour);
      if (ticks <= 0)
         break;
      for (i=0;i<=ticks;i++) {
         p.x = 0;
         p.y = start + i * (stop - start) / ticks;
         p.z = 0;
         q.x = - ticklen;
         q.y = p.y;
         q.z = 0;
         WriteOffVector(fptr,p,q,colour);
         sprintf(s,"%g",startl + i * (stopl - startl) / ticks);
         p.x -= 2.0 * ticklen;
			p.y += ticklen * strlen(s) / 2;
         WriteOffLabel(fptr,s,p,(double)ticklen,'y',colour);
      }
		break;
	case 'z':
      p.x = 0;   
      p.y = 0;
      p.z = start;
      q.x = 0;
      q.y = 0;
      q.z = stop;
      WriteOffVector(fptr,p,q,colour);
      if (ticks <= 0)
         break;
      for (i=0;i<=ticks;i++) {
         p.x = 0;
         p.y = 0;
         p.z = start + i * (stop - start) / ticks;
         q.x = - ticklen / 1.4;
         q.y = - ticklen / 1.4;
         q.z = p.z;
         WriteOffVector(fptr,p,q,colour);
		}
		break;
	}
}

/*
   Wait for Geomview to become ready
*/
void WaitForGeomView(void)
{
   long starttime,thetime;
   char s[256];

   printf("(echo \"ready\n\r\")\n");
   fflush(stdout);
   time((time_t *)&starttime);
   do {
      gets(s);
      time((time_t *)&thetime);
   } while (strcasecmp(s,"ready") != 0 || thetime - starttime > 5);
}
