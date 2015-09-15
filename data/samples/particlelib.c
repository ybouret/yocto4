#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "paulslib.h"
#include "particlelib.h"

/*
   Update the forces on each particle
*/
void CalculateForces(
   PARTICLE *p,int np,
   PARTICLEPHYS phys,
   PARTICLESPRING *s,int ns)
{
   int i,j,p1,p2;
   XYZ down = {0.0,0.0,-1.0};
   XYZ zero = {0.0,0.0,0.0};
   XYZ f;
   double len,dx,dy,dz,scale;

   for (i=0;i<np;i++) {
      p[i].f = zero;
      if (p[i].fixed)
         continue;

      /* Gravity */
      p[i].f.x += phys.gravitation * p[i].mass * down.x;
      p[i].f.y += phys.gravitation * p[i].mass * down.y;
      p[i].f.z += phys.gravitation * p[i].mass * down.z;

      /* Viscous drag */
      p[i].f.x -= phys.viscousdrag * p[i].v.x;
      p[i].f.y -= phys.viscousdrag * p[i].v.y;
      p[i].f.z -= phys.viscousdrag * p[i].v.z;

		/* Electric charge */
		if (phys.coulombconst > 0 && p[i].charge != 0) {
			for (j=0;j<np;j++) {
				if (i == j || p[j].charge == 0)
					continue;
				dx = p[i].p.x - p[j].p.x;
				dy = p[i].p.y - p[j].p.y;
				dz = p[i].p.z - p[j].p.z;
				len = sqrt(dx * dx + dy * dy + dz * dz);
				dx /= len;
      		dy /= len;
      		dz /= len;
				scale = phys.coulombconst * fabs(p[i].charge) * fabs(p[j].charge);
				scale /= (len * len);
				if (p[i].charge * p[j].charge < 0) {	
					p[i].f.x -= dx * scale;
            	p[i].f.y -= dy * scale;
            	p[i].f.z -= dz * scale;
				} else {										
            	p[i].f.x += dx * scale;
            	p[i].f.y += dy * scale;
            	p[i].f.z += dz * scale;
				}
			}
		} 

      /* Gravitational attraction between masses */
      if (phys.gravitationconst > 0 && p[i].mass > 0) {
         for (j=0;j<np;j++) {
            if (i == j || p[j].mass <= 0)
               continue;
            dx = p[i].p.x - p[j].p.x;
            dy = p[i].p.y - p[j].p.y;
            dz = p[i].p.z - p[j].p.z;
            len = sqrt(dx * dx + dy * dy + dz * dz);
            dx /= len;
            dy /= len;
            dz /= len;
            scale = phys.gravitationconst * p[i].mass * p[j].mass;
            scale /= (len * len);
            p[i].f.x -= dx * scale;
            p[i].f.y -= dy * scale;
            p[i].f.z -= dz * scale;
         }
      } 
   }

   /* Handle the spring interaction */
   for (i=0;i<ns;i++) {
      p1 = s[i].from;
      p2 = s[i].to;
      dx = p[p1].p.x - p[p2].p.x;
      dy = p[p1].p.y - p[p2].p.y;
      dz = p[p1].p.z - p[p2].p.z;
      len = sqrt(dx * dx + dy * dy + dz * dz);
		dx /= len;
		dy /= len;
		dz /= len;
      f.x  = s[i].springconstant  * (len - s[i].restlength);
      f.x += s[i].dampingconstant * (p[p1].v.x - p[p2].v.x) * dx;
      f.y  = s[i].springconstant  * (len - s[i].restlength);
      f.y += s[i].dampingconstant * (p[p1].v.y - p[p2].v.y) * dy;
      f.z  = s[i].springconstant  * (len - s[i].restlength);
      f.z += s[i].dampingconstant * (p[p1].v.z - p[p2].v.z) * dz;
      if (!p[p1].fixed) {
         p[p1].f.x -= f.x * dx;
         p[p1].f.y -= f.y * dy;
         p[p1].f.z -= f.z * dz;
      }
      if (!p[p2].fixed) {
         p[p2].f.x += f.x * dx;
         p[p2].f.y += f.y * dy;
         p[p2].f.z += f.z * dz;
      }
   }
}

/*
   Perform one step of the solver
*/
void UpdateParticles(
   PARTICLE *p,int np,
   PARTICLEPHYS phys,
   PARTICLESPRING *s,int ns,
   double dt,int method)
{
   int i;
   PARTICLE *ptmp;
   PARTICLEDERIVATIVES *deriv;

   deriv = (PARTICLEDERIVATIVES *)malloc(np * sizeof(PARTICLEDERIVATIVES));

   switch (method) {
   case 0:                                   /* Euler */
      CalculateForces(p,np,phys,s,ns);
      CalculateDerivatives(p,np,deriv);
      for (i=0;i<np;i++) {
         p[i].p.x += deriv[i].dpdt.x * dt;
         p[i].p.y += deriv[i].dpdt.y * dt;
         p[i].p.z += deriv[i].dpdt.z * dt;
         p[i].v.x += deriv[i].dvdt.x * dt;
         p[i].v.y += deriv[i].dvdt.y * dt;
         p[i].v.z += deriv[i].dvdt.z * dt;
      }
      break;
   case 1:                                   /* Midpoint */
      CalculateForces(p,np,phys,s,ns);
      CalculateDerivatives(p,np,deriv);
      ptmp = (PARTICLE *)malloc(np * sizeof(PARTICLE));
      for (i=0;i<np;i++) {
         ptmp[i] = p[i];
         ptmp[i].p.x += deriv[i].dpdt.x * dt / 2;
         ptmp[i].p.y += deriv[i].dpdt.y * dt / 2;
         ptmp[i].p.z += deriv[i].dpdt.z * dt / 2;
         ptmp[i].p.x += deriv[i].dvdt.x * dt / 2;
         ptmp[i].p.y += deriv[i].dvdt.y * dt / 2;
         ptmp[i].p.z += deriv[i].dvdt.z * dt / 2;
      }
      CalculateForces(ptmp,np,phys,s,ns);
      CalculateDerivatives(ptmp,np,deriv);
      for (i=0;i<np;i++) {
         p[i].p.x += deriv[i].dpdt.x * dt;
         p[i].p.y += deriv[i].dpdt.y * dt;
         p[i].p.z += deriv[i].dpdt.z * dt;
         p[i].v.x += deriv[i].dvdt.x * dt;
         p[i].v.y += deriv[i].dvdt.y * dt;
         p[i].v.z += deriv[i].dvdt.z * dt;
      }
      free(ptmp);
      break;
   }

   free(deriv);
}

/*
   Calculate the derivatives
   dp/dt = v
   dv/dt = f / m
*/
void CalculateDerivatives(
   PARTICLE *p,int np,
   PARTICLEDERIVATIVES *deriv)
{
   int i;

   for (i=0;i<np;i++) {
      deriv[i].dpdt.x = p[i].v.x;
      deriv[i].dpdt.y = p[i].v.y;
      deriv[i].dpdt.z = p[i].v.z;
      deriv[i].dvdt.x = p[i].f.x / p[i].mass;
      deriv[i].dvdt.y = p[i].f.y / p[i].mass;
      deriv[i].dvdt.z = p[i].f.z / p[i].mass;
   }
}

/*
   A 1st order 1D DE solver.
   Assumes the function is not time dependent.
   Parameters
      h      - step size
      y0     - last value
      method - algorithm to use [0,5]
      fcn    - evaluate the derivative of the field
*/
double Solver1D(double h,double y0,int method,double (*fcn)(double))
{
   double k1,k2,k3,k4,k5,k6;
	double ynew = 0;

   switch (method) {
   case 0:                          /* Euler method */
      k1 = h * (*fcn)(y0);
      ynew = y0 + k1;
      break;
   case 1:                          /* Modified Euler */
      k1 = h * (*fcn)(y0);
      k2 = h * (*fcn)(y0 + k1);
      ynew = y0 + (k1 + k2) / 2;
      break;
   case 2:                          /* Heuns method */
      k1 = h * (*fcn)(y0);
      k2 = h * (*fcn)(y0 + 2 * k1 / 3);
      ynew = y0 + k1 / 4 + 3 * k2 / 4;
      break;
   case 3:                          /* Midpoint */
      k1 = h * (*fcn)(y0);
      k2 = h * (*fcn)(y0 + k1 / 2);
      ynew = y0 + k2;
      break;
   case 4:                          /* 4'th order Runge-kutta */
      k1 = h * (*fcn)(y0);
      k2 = h * (*fcn)(y0 + k1/2);
      k3 = h * (*fcn)(y0 + k2/2);
      k4 = h * (*fcn)(y0 + k3);
      ynew = y0 + k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6;
      break;
   case 5:                          /* England 4'th order, six stage */
      k1 = h * (*fcn)(y0);
      k2 = h * (*fcn)(y0 + k1 / 2);
      k3 = h * (*fcn)(y0 + (k1 + k2) / 4);
      k4 = h * (*fcn)(y0 - k2 + 2 * k3);
      k5 = h * (*fcn)(y0 + (7 * k1 + 10 * k2 + k4) / 27);
      k6 = h * (*fcn)(y0 + (28*k1 - 125*k2 + 546*k3 + 54*k4 - 378*k5) / 625);
      ynew = y0 + k1 / 6 + 4 * k3 / 6 + k4 / 6;
      break;
   }

   return(ynew);
}

