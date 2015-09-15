#ifndef OFFLIB_H
#define OFFLIB_H

void   WriteOffLabel(FILE *,char *,XYZ,double,int,COLOUR);
void   WriteOffFacet(FILE *,XYZ *,int,COLOUR);
void   WriteOffCurve(FILE *,XYZ *,int,COLOUR);
void   WriteOffVector(FILE *,XYZ,XYZ,COLOUR);
void   WriteOffPoints(FILE *,XYZ *,int,COLOUR);
void   WriteOffSymbol(FILE *,XYZ,COLOUR,double,int);
void   WriteOffSphere(FILE *,XYZ,double,COLOUR);
void   WriteOffBox(FILE *,XYZ,XYZ,int,COLOUR);
int    WriteOffSurface(FILE *,TRIANGLE *,int,COLOUR);
void   WriteOffAxis(FILE *,char,double,double,double,double,int,double,COLOUR);
void   WaitForGeomView(void);

#endif

