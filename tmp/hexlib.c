#include "hexlib.h"
#include "bitmaplib.h"


/*
	Collection of routines in support of arithmetic on SHM space
	An attempt has been made to make this library self contained,
	that is, it doesn't need external support other than provided
	by standard C ennvironments. Almost all arithmetic is done on
	long ints which are assumed to be at least 4 bytes.
	Paul Bourke
*/

/*
   Return the digit of a base 10 number if it were represented in base 7
	For example: dec 7 = hex 10, first digit is 0, second digit 1
*/
long HexDigit(long b10,long digit)
{
   long b7;

   switch (digit) {
   case  1: b7 = (b10 % 7L);                       break;
   case  2: b7 = (b10 % 49L) / 7L;                 break;
   case  3: b7 = (b10 % 343L) / 49L;               break;
   case  4: b7 = (b10 % 2401L) / 343L;             break;
   case  5: b7 = (b10 % 16807L) / 2401L;           break;
   case  6: b7 = (b10 % 117649L) / 16807L;         break;
   case  7: b7 = (b10 % 823543L) / 117649L;        break;
   case  8: b7 = (b10 % 5764801L) / 823543L;       break;
   case  9: b7 = (b10 % 40353607L) / 5764801L;     break;
   case 10: b7 = (b10 % 282475249L) / 40353607L;   break;
   case 11: b7 = (b10 % 1977326743L) / 282475249L; break;
	default: b7 = 0;
   }

   return(b7);
}

/*
	Return a power of seven, 7^n, where 0 <= n <= 11
	This also gives the number of hexagons in a particular SHM.
	For exaple, a 7^4 SHM has 2401 hexagons.
*/
long HexPower(long n)
{
	long sevens[12] = {1L,7L,49L,343L,2401L,16807L,117649L,
        823534L,5764801L,40353607L,282475249L,1977326743L};

	if (n >= 0 && n <= 11)
		return(sevens[n]);
	else
		return(0);
}

/*
   Determine x-y coordinates in hexagnal space
	This formulation uses a hexagon lying "flat" the distance
   from the center to a vertex is 1. The hexagon of address
	0 is centered at the origin
             +------+         -
            /        \        |
           /          \       |
          +     +      +    sqrt(3)
           \          /       |
            \        /        |
             +------+         -
          |- 1 -|
	Constants used in the code
   	1.04719755119659775        = 60 degrees
   	0.713724378944765631       = atan(sqrt(3)/2)
   	3.141592653589793238462643 = pi
   	1.7320508075688772935      = sqrt(3)
   	4.5826593560436152626      = sqrt(21)
   	2.2913296780218076313      = sqrt(21) / 2
   	2.6457513110645905905      = sqrt(7)
*/
void HexCoord(long b10,double *X,double *Y)
{
   long i;
   double radius,theta;

   *X = 0;
   *Y = 0;
   radius = 1.7320508075688772935;
   for (i=1;i<12;i++) {
      if (i > 1)
         radius *= 2.6457513110645905905;
      if (HexDigit(b10,i) != 0) {
         theta = 3.141592653589793238462643
               + (i - 1) * 0.713724378944765631
               + (HexDigit(b10,i) - 1) * 1.04719755119659775;
         *X += (radius * sin(theta));
         *Y += (radius * cos(theta));
      }
   }
}

/*
   Add two number together in hex space
   a and b are given as decimal numbers.
	Normally this would act as a circular addition.
	Straight addition can be performed by using a large resol value, eg: 10.
*/
long HexAdd(long a10,long b10,long resol)
{
   long addtable[7][7] =
      { {0, 1, 2, 3, 4, 5, 6},
        {1,63,15, 2, 0, 6,64},
        {2,15,14,26, 3, 0, 1},
        {3, 2,26,25,31, 4, 0},
        {4, 0, 3,31,36,42, 5},
        {5, 6, 0, 4,42,41,53},
        {6,64, 1, 0, 5,53,52} };
   long sevens[12] = {1L,7L,49L,343L,2401L,16807L,117649L,
        823534L,5764801L,40353607L,282475249L,1977326743L};
   long i,j,t,carry,ta,tb;
   long da[12],db[12];
   long total[12];
   long sum = 0;

   /* Load up the digits in the reverse order */
   ta = a10;
   tb = b10;
   for (i=0;i<12;i++) {
      da[i] = ta % 7;
      db[i] = tb % 7;
      ta /= 7;
      tb /= 7;
      total[i] = 0;
   }

   /* Do the addition */
   for (i=0;i<12;i++) {
      t = addtable[da[i]][db[i]];
      total[i] = t % 10;
      carry = t / 10;
      for (j=i+1;j<12;j++) {
         if (carry <= 0)
            break;
         t = addtable[da[j]][carry];
         da[j] = t % 10;
         carry = t / 10;
      }
   }

   /* Calculate the sum */
   for (i=0;i<12;i++)
      sum += (total[i] * sevens[i]);
   return(sum % sevens[resol]);
}

/*
   Same as HexAdd() except a and b are given in base 7
*/
long HexAdd_b7(long a7,long b7,long resol)
{
   return(HexAdd(Hex2Dec(a7),Hex2Dec(b7),resol));
}

/*
   Multiply in hex space
   a and b are given as decimal numbers
	Normally this perfosm a circular multiply. For a straight
	multiply choose a large value of resol. Note there is a limit
	to the size of a straight multiply as the result must be less
	that base 12, thus the operands must be less that base 7.
*/
long HexMul(long a10,long b10,long resol)
{
   long i,j,k,ta,tb;
   long da[12],db[12];
   long total[12];
   long sum = 0;
   long sums[12];       /* Partial sums in decimal */

   long multable[7][7] =
      { {0,0,0,0,0,0,0},
        {0,1,2,3,4,5,6},
        {0,2,3,4,5,6,1},
        {0,3,4,5,6,1,2},
        {0,4,5,6,1,2,3},
        {0,5,6,1,2,3,4},
        {0,6,1,2,3,4,5}};
   long sevens[12] = {1L,7L,49L,343L,2401L,16807L,117649L,
        823534L,5764801L,40353607L,282475249L,1977326743L};

   /*
      Shift a left (x10) and b right (/10) until the low digit of b isn't 0
   */
   ta = a10;
   tb = b10;
   while (tb != 0 && (tb % 7) == 0) {
      ta = 7 * (ta % sevens[resol-1]) + (ta / sevens[resol-1]);
      tb = tb / 7 + (tb % 7) * sevens[resol-1];
   }

   /* Load up the digits in the reverse order */
   for (i=0;i<12;i++) {
      da[i] = ta % 7;
      db[i] = tb % 7;
      ta /= 7;
      tb /= 7;
      sums[i] = 0;
   }

   /* Calculate the partial sums */
   for (i=0;i<resol;i++) {

      for (j=0;j<12;j++)
         total[j] = 0;

      for (j=0;j<resol;j++) {
         k = i + j;
         if (k < 12)
            total[k] = multable[db[i]][da[j]];
      }

      for (j=0;j<resol;j++)
         sums[i] += (total[j] * sevens[j]);
   }

   /* Add up all the partial sums */
   for (i=0;i<resol;i++)
      sum = HexAdd(sum,sums[i],resol);

   return(sum % sevens[resol]);
}

/*
   Same as HexMul() except a and b are given in base 7
*/
long HexMul_b7(long a7,long b7,long resol)
{
   return(HexMul(Hex2Dec(a7),Hex2Dec(b7),resol));
}

/*
	Shift left (whichway == 1) or right (whichway = 2) circular
	a number of times
	Can be done with multiplications but this is often much faster
*/
long HexShift(long a10, int whichway, int howmany, long resol)
{
	int i;
	long ans;
   long sevens[12] = {1L,7L,49L,343L,2401L,16807L,117649L,
        823534L,5764801L,40353607L,282475249L,1977326743L};

	resol--;
	if (resol < 0)
		return(0);
	ans = a10;
	for (i=0;i<howmany;i++) {
		switch (whichway) {
		case 1:
			ans = 7 * (ans % sevens[resol]) + ans / sevens[resol];
			break;
		case 2:
			ans = ans / 7 + (ans % 7) * sevens[resol];
			break;
		}
	}
	return(ans);
}

/*
   Convert a decimal number into hex digits
*/
long Dec2Hex(long a10)
{
   long hex,ten;

   hex = 0;
   ten = 1;
   while (a10 > 0) {
      hex += ten * (a10 % 7);
      ten *= 10;
      a10 /= 7;
   }

   return(hex);
}

/*
   Convert hex digits into a decimal
*/
long Hex2Dec(long a7)
{
   long dec,seven;

   dec = 0;
   seven = 1;
   while (a7 > 0) {
      dec += seven * (a7 % 10);
      seven *= 7;
      a7 /= 10;
   }

   return(dec);
}

/*
	Return TRUE if a cell is on an even column (row)
*/
long HexEven(long dec)
{
	int hindex;
	double x,y;

	HexCoord(dec,&x,&y);
	hindex = rint(ABS(x) / 1.5);
	if (hindex % 2 == 0)
		return(TRUE);
	else
		return(FALSE);
}

/*
   Return TRUE if a cell is on the left semicircle
	Cells at x=0 are on the left if above 0
	0 and cells below 0 are on the right
*/
long HexLeft(long dec)
{
   double x,y;

   HexCoord(dec,&x,&y);
   if (x < -0.1)
      return(TRUE);
	else if (x > 0.1)
		return(FALSE);
	else if (y > 0.1)
		return(TRUE);
   else
      return(FALSE);
}

/*
   Perform a reflection of an SHM address a (in decimal)
   about either 'x' or 'y' axis. For a particular base not
   all addresses will have a reflected counterpart, this
   routine returns -1 in those cases.
   Note: this is VERY inefficient, do it once for a reflection table
*/
long HexReflect(long a10,long base,int axis)
{
   long i;
   double d,dmin = 1e32;
   long amin = -1;
   double x,y,cx,cy;

   HexCoord(a10,&cx,&cy);
   for (i=0;i<HexPower(base);i++) {
      HexCoord(i,&x,&y);
      if (axis == 'x' || axis == 'X')
         y = -y;
      else
         x = -x;
      d = (x - cx) * (x - cx) + (y - cy) * (y - cy);
      if (d < dmin) {
         dmin = d;
         amin = i;
      }
   }
   if (dmin <= 1)
      return(amin);
   else
      return(-1);
}

/*
   Find the "magnitude" of a hex address
	There 1288 vertical hexagons for a 7^8 SHM
*/
long HexMagnitude(long a10)
{
   long i,sum = 0,imin=0;
   double x,y,x1,y1;
   double dist0,dist,dmin = 1e32;

   HexCoord(a10,&x,&y);
   dist0 = sqrt(x * x + y * y);

   for (i=0;i<1288;i++) {
      HexCoord(sum,&x1,&y1);
		dist = dist0 - sqrt(x1 * x1 + y1 * y1);
      if (fabs(dist) < dmin) {
         dmin = dist;
			imin = sum;
      }
		if (dist + sqrt(3.0) < 0)
			break;
      sum = HexAdd(sum,1L,10);
   }
   return(imin);
}

/*
	Return the Euclidean distance between two addresses
*/
double HexDistance(long a1,long a2)
{
	double x1,y1,x2,y2,dx,dy;

	HexCoord(a1,&x1,&y1);
	HexCoord(a2,&x2,&y2);
	dx = x2 - x1;
	dy = y2 - y1;

	return(sqrt(dx*dx + dy*dy));
}

/*
	Return the euclidean angle between two addresses
*/
double HexAngle(long a1,long a2)
{
	double x1,y1,x2,y2,dx,dy;

   HexCoord(a1,&x1,&y1);
   HexCoord(a2,&x2,&y2);
   dx = x2 - x1;
   dy = y2 - y1;

	return(atan2(dy,dx));
}

/*
   "Find" the inverse of a hex multiplicative address
   Return 0 if an inverse doesn't exist
   VERY inefficient, use it to make a table!
*/
long HexInverse(long a10,long base,long basemax)
{
   long i;

   if (a10 <= 0)
      return(0);

   for (i=0;i<HexPower(basemax);i++) {
      if (HexMul(a10,i,base) == 1)
         return(i);
   }
   return(0);
}

/*
	Return the "next" sqrt of an address
	Since multiple square roots can exist for a particular base
	this routine is designed to be called multiple times until
	it returns -1 (no sqrt). On the first call "start" would be
	-1, on subsequent calls "start" would be one more than the
	last square root found.
	For example
      lastroot = -1;
      while ((lastroot = HexSqrt(i,lastroot+1,4)) >= 0)
         printf("%ld\t",lastroot);

*/
long HexSqrt(long a10,long start,long base)
{
	long i;

   i = start;
	while (i < HexPower(base)) {
      if (HexMul(i,i,base) == a10) 
         return(i);
		i++;
   }
	return(-1);
}

/*
	Complex powers
	This is based on a geometric interpretation, same radius, angle*power
	In particular power = 2 for square, power = 0.5 for squareroot
	In all cases the "closest" hex cell is returned.
	Return -1 of out of bounds
*/
long HexComplexPower(long a10,double power,long base)
{
	long j;
	double r,theta,x,y;

	HexCoord(a10,&x,&y);
   r = sqrt(x*x + y*y);
   theta = power * atan2(y,x);
   x = r * cos(theta);
   y = r * sin(theta);
   j = CoordHex(x,y,base+1);
	if (j < HexPower(base))
		return(j);
	else
		return(-1L);
}

/*
	Given a cartesian coordinate find the closest hex cell
	Very inefficient, does a linear search using HexCoord()
	Assumes you know the maximum range of your addresses
	Returns -1 if the closest is outside the SHM of 7^base
*/
long CoordHex(double x, double y, long base)
{
	long i,closest=-1;
	double cx,cy;
	double dx,dy,dist,mindist=1e32;

	for (i=0;i<HexPower(base);i++) {
		HexCoord(i,&cx,&cy);
		dx = cx - x;
		dy = cy - y;
		dist = dx*dx + dy*dy;
		if (dist < mindist) {
			mindist = dist;
			closest = i;
			if (mindist < 0.866)
				break;
		}	
	}
	if (mindist > 1)
		closest = -1;
	return(closest);
}

/*
	Write a hex cell in OFF format specifying its colour and whether or not
	it is filled (or wireframe). OOGL/OFF is a 3D format, the hexagon is
	laid on the x-y plain (z = 0). Uses the conventions in HexCoord()
	eg: for GeomView
*/
void HexOff(
	FILE *fptr,
	long a10,
	double red,double green,double blue,
	int filled)
{
	double cx,cy;

	HexCoord(a10,&cx,&cy);

	if (filled) {
   	fprintf(fptr,"{ = OFF\n");
   	fprintf(fptr,"\t6 1 1\n");
   	fprintf(fptr,"\t%g %g 0\n",cx-1.0,cy);
		fprintf(fptr,"\t%g %g 0\n",cx-0.5,cy+sqrt(3.0)/2);
   	fprintf(fptr,"\t%g %g 0\n",cx+0.5,cy+sqrt(3.0)/2);
   	fprintf(fptr,"\t%g %g 0\n",cx+1.0,cy);
   	fprintf(fptr,"\t%g %g 0\n",cx+0.5,cy-sqrt(3.0)/2);
   	fprintf(fptr,"\t%g %g 0\n",cx-0.5,cy-sqrt(3.0)/2);
   	fprintf(fptr,"\t6 0 1 2 3 4 5 %g %g %g 1\n",red,green,blue);
   	fprintf(fptr,"}\n");
	} else {
      fprintf(fptr,"{ = SKEL\n");
      fprintf(fptr,"\t6 1\n");
      fprintf(fptr,"\t%g %g 0\n",cx-1.0,cy);
      fprintf(fptr,"\t%g %g 0\n",cx-0.5,cy+sqrt(3.0)/2);
      fprintf(fptr,"\t%g %g 0\n",cx+0.5,cy+sqrt(3.0)/2);
      fprintf(fptr,"\t%g %g 0\n",cx+1.0,cy);
      fprintf(fptr,"\t%g %g 0\n",cx+0.5,cy-sqrt(3.0)/2);
      fprintf(fptr,"\t%g %g 0\n",cx-0.5,cy-sqrt(3.0)/2);
      fprintf(fptr,"\t7 0 1 2 3 4 5 0 %g %g %g 1\n",red,green,blue);
      fprintf(fptr,"}\n");
	}
}

/*
	Create storage for a SHM
*/
int CreateSHM(double *shm,long degree)
{
	if ((shm = malloc(HexPower(degree)*sizeof(double))) == NULL)
		return(FALSE);
	else
		return(TRUE);
}

/*
	Destroy the SHM structure
*/
void DestroySHM(double *shm)
{
	free(shm);
}

/*
	Initialise a SHM structure
*/
void InitialiseSHM(double *shm,long degree,double value)
{
	long i;

	for (i=0;i<HexPower(degree);i++)
		shm[i] = value;
}

/*
   Add a constant to every cell of an SHM
*/
void AddSHM(double *shm,long degree,double value)
{
   long i;

   for (i=0;i<HexPower(degree);i++)
      shm[i] += value;
}

/*
	Multiply each cell in an SHM by a constant
*/
void MulSHM(double *shm,long degree,double value)
{
   long i;

   for (i=0;i<HexPower(degree);i++)
      shm[i] *= value;
}

/*
	Copy shm1 to shm2
*/
void CopySHM(double *shm1, double *shm2, long degree)
{
	long i;

	for (i=0;i<HexPower(degree);i++)
		shm2[i] = shm1[i];
}

/*
	Compute the minimum and maximum values in an SHM
*/
void BoundsSHM(double *shm,long degree,double *themin,double *themax)
{
	long i;

	*themin = shm[0];
	*themax = shm[0];
	for (i=1;i<HexPower(degree);i++) {
		if (shm[i] < *themin)
			*themin = shm[i];
		if (shm[i] > *themax)
			*themax = shm[i];
	}
}

/*
	Create a weight matrix structure
	Set the number of initial maximum number of input weights to 100
*/
CONNECTLIST *CreateWeightMatrix(int degree)
{
	long i,n;
	CONNECTLIST *connect;

	n = HexPower(degree);
	if ((connect = (CONNECTLIST *)malloc(n * sizeof(CONNECTLIST))) == NULL)
		return(NULL);

	for (i=0;i<n;i++) {
		if ((connect[i].in = (int *)malloc(100 * sizeof(int))) == NULL) {
			free(connect);
			return(NULL);
		}
      if ((connect[i].weight = (float *)malloc(100 * sizeof(float))) == NULL) {
         free(connect);
         return(NULL);
      }
      if ((connect[i].delay = (int *)malloc(100 * sizeof(int))) == NULL) {
         free(connect);
         return(NULL);
      }
		connect[i].maxin = 100;
		connect[i].currin = 0;
	}

	return(connect);
}

/*
	Add a weight to the connectivity list
*/
int AddtoWeightMatrix(
	CONNECTLIST *connect,int from,int to,float weight,int delay,int degree)
{
	long i,n,newin;

	n = HexPower(degree);
	if (to < 0 || to >= n)
		return(FALSE);
   if (from < 0 || from >= n)
      return(FALSE);

	/* 	
		Search through each incoming connection
		The from connection and the delay must match
		If so, increase the weight
		Note: it is possible to have two paths between the same two patches
	*/
	for (i=0;i<connect[to].currin;i++) {
		if (connect[to].in[i] == from && connect[to].delay[i] == delay) {
			connect[to].weight[i] += weight;
			return(TRUE);
		}
	}

	/* Do we need to allocate more space */
	if (connect[to].currin >= connect[to].maxin - 1) {
		newin = connect[to].maxin + 100;
		if ((connect[to].in = 
			(int *)realloc(connect[to].in,newin*sizeof(int))) == NULL) 
				return(FALSE);
      if ((connect[to].weight = 
			(float *)realloc(connect[to].weight,newin*sizeof(float))) == NULL)
         	return(FALSE);
      if ((connect[to].delay =
         (int *)realloc(connect[to].delay,newin*sizeof(int))) == NULL)
            return(FALSE);
		connect[to].maxin = newin;
	}

	/* Finally, add the new connection */
	newin = connect[to].currin;
	connect[to].in[newin] = from;
	connect[to].weight[newin] = weight;
	connect[to].delay[newin] = delay;
	connect[to].currin++;

	return(TRUE);
}

/*
	Return the number of connections
*/
int CountWeightMatrix(CONNECTLIST *connect,int degree)
{
	int count = 0;
	int i,n;

	n = HexPower(degree);
	
	for (i=0;i<n;i++) 
		count += connect[i].currin;

	return(count);
}

/* --------------------------------------------------------------
   Sample a bar onto an SHM matrix
	Return the number of points sampled 
		0 means the bar is outside the SHM bounds

   shm    - double array representing the shm[0..7^base7]
	dist   - distance from the center of the shm
              for a moving bar this would range from +- size of shm
	theta  - angle from the positive x axis (radians)
	width  - width of the bar
   base7  - base of the shm
   resol7 - base of the resolution at which to sample the bar, 0..base7
*/
int SampleBarOnSHM(double *shm,
					double dist,double theta,double width,
					int base7, int resol7)
{
   int i,j,k;
	int count = 0;
   double x,y,x1,y1,x2,y2,x3,y3,mu,dx,dy;
	double dw;
   long sevens[12] = {1L,7L,49L,343L,2401L,16807L,117649L,
        823534L,5764801L,40353607L,282475249L,1977326743L};

	dw = width * width / 4;

   /* Determine the two end points of the bar */
   x = -1;
   y = dist;
   x1 = x * cos(theta) + y * sin(theta);
   y1 = y * cos(theta) - x * sin(theta);
   x = 1;
   y = dist;
   x2 = x * cos(theta) + y * sin(theta);
   y2 = y * cos(theta) - x * sin(theta);

   /* Erase the layer */
   for (k=0;k<sevens[base7];k++)
      shm[k] = 0;

   /* Sample onto the hex grid */
   for (i=0;i<sevens[base7];i+=sevens[resol7]) {
      HexCoord((long)i,&x3,&y3);
      dx = x2 - x1;
      dy = y2 - y1;
      mu = (x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1);
      mu /= (dx*dx + dy*dy);
      x = x1 + mu * dx;
      y = y1 + mu * dy;
      dx = x3 - x;
      dy = y3 - y;
      if (dx * dx + dy * dy >= dw)
         continue;

      for (j=0;j<sevens[resol7];j++) {
         k = HexAdd((long)i,(long)j,8L);
         if (k < 0 || k > sevens[base7])
            continue;
         shm[k] = 1;
			count++;
      }
   }

	return(count);
}

/* --------------------------------------------------------------
   Write an SHM as a ppm image
*/
int WriteSHMtoPPM(
	char *ppmfilename,
	int n,							/* Size of the image (square)				*/
	double *l,						/* The SHM data								*/
	int base7,						/* The base of the SHM						*/
	double lmin,double lmax,	/* Bounds at which to clip the image	*/
	int maptype,					/* Colour map type, see GetColour		*/
	int bg_r,int bg_g,int bg_b	/* Background colour 0..255				*/
	)
{
   int i,ix,iy;
   double cx,cy,ystart,x,y;
	double xmin=1e32,xmax=-1e32,ymin=1e32,ymax=-1e32;
	double scale;
	COLOUR colour;
   FILE *fptr;
   BITMAP4 *img;
	BITMAP4 c;
   long sevens[12] = {1L,7L,49L,343L,2401L,16807L,117649L,
        823534L,5764801L,40353607L,282475249L,1977326743L};

   /* Create the image file */
   if ((fptr = fopen(ppmfilename,"wb")) == NULL) 
      return(FALSE);

   /* Create the image */
   img = Create_Bitmap(n,n);
	c.r = bg_r;
	c.b = bg_b;
	c.g = bg_g;
   Erase_Bitmap(img,n,n,c);

   /* Find coordinate ranges (Inefficient) */
   for (i=0;i<sevens[base7];i++) {
		HexCoord((long)i,&cx,&cy);
		xmin = MIN(xmin,cx);
		xmax = MAX(xmax,cx);
      ymin = MIN(ymin,cy);
      ymax = MAX(ymax,cy);
   }
	scale = 0.95 * MIN(n / (xmax - xmin), n / (ymax - ymin));

   /* Form the image */
   for (i=0;i<sevens[base7];i++) {
		colour = GetColour(l[i],lmin,lmax,maptype);
      c.r = 255 * colour.r;
      c.g = 255 * colour.g;
      c.b = 255 * colour.b;
      HexCoord((long)i,&cx,&cy);

      for (x=-scale-0.9;x<=1.1;x++) {
         ystart = 1.7320508075688772935 * (x / scale + 1);
         if (ystart > 0.866025404)
            ystart = 0.866025404;
         ystart *= scale;
         for (y=ystart+0.9;y>-ystart-0.9;y--) {
            ix = cx * scale + x + n / 2;
            iy = cy * scale + y + n / 2;
            Draw_Pixel(img,n,n,ix,iy,c);
            ix = cx * scale - x + n / 2;
            iy = cy * scale + y + n / 2;
            Draw_Pixel(img,n,n,ix,iy,c);
         }
      }
   }

   /* Write the image and end */
   Write_Bitmap(fptr,img,n,n,2);
   fclose(fptr);
   Destroy_Bitmap(img);
	return(TRUE);
}

/* --------------------------------------------------------------
   Sample a bar onto an SHM matrix
   This version supports bars with length and offset from the origin.
   Same as SampleBarOnSHM() if length -> infinity and distx = 0.

   Algorithm: Place a bar of length "length" width "width" 
              vertically at the origin.
              Displace vertically by "disty"
              Displace horizontally by "distx"
              Rotate by "theta"
	The distance from the middle of the line to the center of the
   SHM is therefore sqrt(distx*distx + disty*disty)

   Return the number of points sampled
      0 means the bar is outside the SHM bounds

   shm    - double array representing the shm[0..7^base7]
   disty  - for a moving bar this would range from +- size of shm
   theta  - angle from the positive x axis (radians)
   width  - width of the bar in SHM units
   distx  - this would normally range from +- size of shm
   length - length of the bar in SHM units
   base7  - base of the shm
   resol7 - base of the resolution at which to sample the bar, 0..base7
*/
int SampleBarOnSHM2(double *shm,
               double disty, double theta, double width,
               double distx, double length,
               int base7, int resol7)
{
   int i,j,k;
   int count = 0;
   double x,y,x1,y1,x2,y2,x3,y3,mu,dx,dy;
   double dw,dl;
   long sevens[12] = {1L,7L,49L,343L,2401L,16807L,117649L,
        823534L,5764801L,40353607L,282475249L,1977326743L};

   dw = width * width / 4;
   dl = length / 2;

   /* Determine the two end points of the bar */
   x = distx - dl;
   y = disty;
   x1 = x * cos(theta) + y * sin(theta);
   y1 = y * cos(theta) - x * sin(theta);
   x = distx + dl;
   y = disty;
   x2 = x * cos(theta) + y * sin(theta);
   y2 = y * cos(theta) - x * sin(theta);

   /* Erase the layer */
   for (k=0;k<sevens[base7];k++)
      shm[k] = 0;

   /* Sample onto the hex grid */
   for (i=0;i<sevens[base7];i+=sevens[resol7]) {
      HexCoord((long)i,&x3,&y3);
      dx = x2 - x1;
      dy = y2 - y1;
      mu = (x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1);
      mu /= (dx*dx + dy*dy);
      if (mu < 0 || mu > 1)
         continue;
      x = x1 + mu * dx;
      y = y1 + mu * dy;
      dx = x3 - x;
      dy = y3 - y;
      if (dx * dx + dy * dy >= dw)
         continue;

      for (j=0;j<sevens[resol7];j++) {
         k = HexAdd((long)i,(long)j,8L);
         if (k < 0 || k > sevens[base7])
            continue;
         shm[k] = 1;
         count++;
      }
   }

   return(count);
}



