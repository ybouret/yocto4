#ifndef PARTICLELIB_H
#define PARTICLELIB_H

typedef struct {
   double mass;  					/* Mass                          				*/
   XYZ p;      					/* Position                      				*/
   XYZ v;      					/* Velocity                      				*/
   XYZ f;      					/* Force                         				*/
   int fixed;  					/* Fixed point or free to move   				*/
	double charge;					/* Charge for electrostatic force				*/
} PARTICLE;

typedef struct {
   XYZ dpdt;
   XYZ dvdt;
} PARTICLEDERIVATIVES;

typedef struct {
   double gravitation;			/* Downward gravitational acceleration			*/
   double viscousdrag;			/* Viscous drag, proportional to velocity		*/
	double coulombconst;			/* Coulombs constant for electrostatic force	*/
	double gravitationconst;	/* Gravitational constant between 2 masses	*/
} PARTICLEPHYS;

typedef struct {
   int from;						/* Particle at one end of the spring			*/
   int to;							/* Particle at the other end of the spring	*/
   double springconstant;		/* Spring constant for Hooks law					*/
   double dampingconstant;		/* Damping coefficient for Hooks law			*/
   double restlength;			/* Rest length for the spring						*/
} PARTICLESPRING;

void CalculateForces(PARTICLE *,int,
							PARTICLEPHYS,PARTICLESPRING *,int);
void UpdateParticles(PARTICLE *,int,
							PARTICLEPHYS,PARTICLESPRING *,int,double,int);
void CalculateDerivatives(PARTICLE *,int,
							PARTICLEDERIVATIVES *);
double Solver1D(double,double,int,double (*)(double));

#endif
