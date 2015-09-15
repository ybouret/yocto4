#ifndef WRLLIB_H
#define WRLLIB_H

void WRLLine(FILE *,XYZ *,int,COLOUR);
void WRLLine2(FILE *,XYZ,XYZ,COLOUR);
void WRLSphere(FILE *,XYZ,double,COLOUR);
void WRLSphere2(FILE *,XYZ,double,int,COLOUR);
void WRLCone(FILE *,XYZ,XYZ,double,double,int,COLOUR,int);
void WRLCone2(FILE *,XYZ,XYZ,double,double,int,COLOUR,int);
void WRLFace(FILE *,XYZ *,int,COLOUR);
void WRLHeader(FILE *,char *,double);

#endif /* WRLLIB_H */
