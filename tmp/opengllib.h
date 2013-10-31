#ifndef OPENGLLIB_H
#define OPENGLLIB_H

typedef struct {
	int id;
	char name[128];
} PANELITEM;
typedef struct {
	int id;
	int nitems;
	PANELITEM *items;
} PANEL;

int WindowDump(char *,int,int,int,int);
void ClearBuffers(int);
void ClearAllBuffers(void);
void CreateBox(XYZ,XYZ,int);
void CreateRegularGrid(XYZ,XYZ,int);
void CreatePolygon(XYZ *,XYZ *,COLOUR *,int);
void CreateSphere(XYZ,double,int,int,double,double,double,double);
void CreateSimpleSphere(XYZ,double,int,int);
void CreateSuperEllipse(double,double,int,int);
void EvalSuperEllipse(double,double,double,double,XYZ *);
void CreateEllipsoid(XYZ,double,int,double,double,double);
void CreateDisk(XYZ,XYZ,double,double,int,double,double);
void CreateCone(XYZ,XYZ,double,double,int,double,double);
void CreateTorus(XYZ,double,double,int,int,double,double,double,double,int);
void CreateStars(XYZ,double);
void CreateRandomStars(int);
BITMAP4 *ReadRawTexture(int,int,char *,COLOUR,int);
BITMAP4 *ReadPPMTexture(char *,COLOUR,int,int *,int *,int);
BITMAP4 *ReadTGATexture(char *,int *,int *);
BITMAP4 *ReadRawTexture2(int,int,char *,int);
void DrawGLText(int,int,char *);
void DrawTextXY(double,double,double,double,char *);
void CreateLabelVector(char *,XYZ,XYZ,XYZ,XYZ *,int *);
void Draw3DPlus(XYZ,double);
void DrawSimpleAxes(XYZ,double);
void DrawAxes(XYZ,XYZ,int,int);
void Create3DCursor(int,int,int,int,XYZ,XYZ,XYZ,double,double);
void DrawBlueLine(int,int,int);

#endif /* OPENGLLIB_H */
