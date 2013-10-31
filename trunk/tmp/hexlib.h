#ifndef HEXLIB_INCLUDED
#define HEXLIB_INCLUDED 1

#include "paulslib.h"

typedef struct {
	int maxin;		/* Number of input connections allocated 			*/
	int currin;		/* Number of input connections being used 		*/
	int *in;			/* List of the input connections 					*/
	float *weight;	/* The actual weight value 							*/
	int *delay;		/* Delay for this input in integer time steps	*/
} CONNECTLIST;

long   HexDigit(long,long);
long   HexPower(long);
void   HexCoord(long,double *,double *);
long   HexAdd(long,long,long);
long   HexAdd_b7(long,long,long);
long   HexMul(long,long,long);
long   HexMul_b7(long,long,long);
long   HexShift(long,int,int,long);
long   Hex2Dec(long);
long   Dec2Hex(long);
long   HexEven(long);
long   HexLeft(long);
long   HexReflect(long,long,int);
long   HexMagnitude(long);
double HexDistance(long,long);
double HexAngle(long,long);
long   HexInverse(long,long,long);
long   HexSqrt(long,long,long);
long   HexComplexPower(long,double,long);
long   CoordHex(double,double,long);
void   HexOff(FILE *,long,double,double,double,int);
int    CreateSHM(double *,long);
void   DestroySHM(double *);
void   InitialiseSHM(double *,long,double);
void   AddSHM(double *,long,double);
void   MulSHM(double *,long,double);
void   CopySHM(double *,double *,long);
void   BoundsSHM(double *,long,double *,double *);

CONNECTLIST *CreateWeightMatrix(int);
int AddtoWeightMatrix(CONNECTLIST *,int,int,float,int,int);
int CountWeightMatrix(CONNECTLIST *,int);

int SampleBarOnSHM(double *,double,double,double,int,int);
int SampleBarOnSHM2(double *,double,double,double,double,double,int,int);
int WriteSHMtoPPM(char *,int,double *,int,double lmin,double,int,int,int,int);

#endif

