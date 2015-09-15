#include "complexlib.h"

/*
   Cadd(z1,z2) = z1 + z2
*/
COMPLEX Cadd(COMPLEX z1,COMPLEX z2)
{
    COMPLEX ztmp;

    ztmp.real = z1.real + z2.real;
    ztmp.imag = z1.imag + z2.imag;
    return(ztmp);
}

/*
   Csub(z1,z2) = z1 - z2
*/
COMPLEX Csub(COMPLEX z1,COMPLEX z2)
{
    COMPLEX ztmp;

    ztmp.real = z1.real - z2.real;
    ztmp.imag = z1.imag - z2.imag;
    return(ztmp);
}

/*
   Cmult(z1,z2) = z1 * z2
*/
COMPLEX Cmult(COMPLEX z1,COMPLEX z2)
{
    COMPLEX ztmp;

    ztmp.real = z1.real * z2.real - z1.imag * z2.imag;
    ztmp.imag = z1.real * z2.imag + z2.real * z1.imag;
    return(ztmp);
}

/*
   Cmultd(z,d) = z * d
*/
COMPLEX Cmultd(COMPLEX z,double d)
{
    COMPLEX ztmp;

    ztmp.real = z.real * d;
    ztmp.imag = z.imag * d;
    return(ztmp);
}

/*
   Csqrt(z) = u (+-) jv
   where u = sqrt(0.5*(root+x)) and v = sqrt(0.5*(root-x))
   and root is the magnitude of z
   the sign is the same as that of the imaginary part of z
*/
COMPLEX Csqrt(COMPLEX z)
{
    COMPLEX ztmp;
    double root;

    if (z.real == 0.0 && z.imag == 0.0) {
        ztmp.real = 0.0;
        ztmp.imag = 0.0;
    } else if (z.real >= 0 && z.imag == 0.0) {
        ztmp.real = sqrt(z.real);
        ztmp.imag = 0.0;
    } else {
        root = sqrt(z.real*z.real + z.imag*z.imag);
        ztmp.real = sqrt(0.5 * (root + z.real));
        ztmp.imag = sqrt(0.5 * (root - z.real));
        if (z.imag < 0.0)
            ztmp.imag = - ztmp.imag;
    }
    return(ztmp);
}

/*
   Natural logarithm of a complex number
*/
COMPLEX Clog(COMPLEX z)
{
    COMPLEX ztmp;

    if (z.imag == 0.0 && z.real > 0.0) {
        ztmp.real = log(z.real);
        ztmp.imag = 0.0;
    } else if (z.real == 0.0) {
        if (z.imag > 0.0) {
            ztmp.real = log(z.imag);
            ztmp.imag = PID2_value;
        } else {
            ztmp.real = log(-(z.imag));
            ztmp.imag = - PID2_value;
        }
    } else {
        ztmp.real = log(sqrt(z.real*z.real + z.imag*z.imag));
        ztmp.imag = atan2(z.imag,z.real);
    }
    return(ztmp);
}

/*
   Cexp(z) = exp(real) cos(imag) + j( exp(real) sin(imag) )
   where z = real + j imag
*/
COMPLEX Cexp(COMPLEX z)
{
    double r;
    COMPLEX ztmp;

    r = exp(z.real);
    ztmp.real = r * cos(z.imag);
    ztmp.imag = r * sin(z.imag);
    return(ztmp);
}

/*
  Csin(z) = sin(real) cosh(imag) + j cos(real) sinh(imag)
*/
COMPLEX Csin(COMPLEX z)
{
    COMPLEX ztmp;

    if (z.imag == 0.0) {
        ztmp.real = sin(z.real);
        ztmp.imag = 0.0;
    } else {
        ztmp.real = sin(z.real) * cosh(z.imag);
        ztmp.imag = cos(z.real) * sinh(z.imag);
    }
    return(ztmp);
}

/*
   Ccos(z) = cos(real) cosh(imag) - j sin(real) sinh(imag)
*/
COMPLEX Ccos(COMPLEX z)
{
    COMPLEX ztmp;

    if (z.imag == 0.0) {
        ztmp.real = cos(z.real);
        ztmp.imag = 0.0;
    } else {
        ztmp.real =   cos(z.real) * cosh(z.imag);
        ztmp.imag = - sin(z.real) * sinh(z.imag);
    }
    return(ztmp);
}

/*
  Ctan(z) = ( sin(2*real) + jsinh(2*imag) )
            -------------------------------
            ( cos(2*real) + cosh(2*imag) )
*/
COMPLEX Ctan(COMPLEX z)
{
    COMPLEX ztmp;
    double denom,real2,imag2;

    if (z.imag == 0.0) {
        ztmp.real = tan(z.real);
        ztmp.imag = 0.0;
    } else {
        real2 = 2.0 * z.real;
        imag2 = 2.0 * z.imag;
        denom = cos(real2) + cosh(imag2);
        ztmp.real = sin(real2) / denom;
        ztmp.imag = sinh(imag2) / denom;
    }
    return(ztmp);
}

/*
   Casin(z) = k*pi + (-1)^k asin(b)
                   + j (-1)^k log(a + sqrt(a^2 - 1))
   where a = 0.5 sqrt((x+1)^2 + y^2) + 0.5 sqrt((x-1)^2 + y^2)
   and   b = 0.5 sqrt((x+1)^2 + y^2) - 0.5 sqrt((x-1)^2 + y^2)
   and z = x + jy, k an integer
*/
COMPLEX Casin(COMPLEX z)
{
    COMPLEX ztmp;
    double a,b;
    double xm1,xp1,x2,y2;
    double part1,part2;

    if (z.imag == 0.0) {
        ztmp.real = asin(z.real);
        ztmp.imag = 0.0;
    } else {
        x2 = z.real * z.real;
        y2 = z.imag * z.imag;
        xp1 = x2 + 2.0 * z.real + 1.0;
        xm1 = x2 - 2.0 * z.real + 1.0;
        part1 = 0.5 * sqrt(xp1 + y2);
        part2 = 0.5 * sqrt(xm1 + y2);
        a = part1 + part2;
        b = part1 - part2;
        ztmp.real = asin(b);
        ztmp.imag = log(a + sqrt(a * a - 1.0) );
    }
    return(ztmp);
}

/*
   Cacos(z) = 2*k*pi (+-) [ acos(b)
                       - j log(a + sqrt(a^2 - 1))
   where a = 0.5 sqrt((x+1)^2 + y^2) + 0.5 sqrt((x-1)^2 + y^2)
   and   b = 0.5 sqrt((x+1)^2 + y^2) - 0.5 sqrt((x-1)^2 + y^2)
   and   z = x + jy, K an integer.
*/
COMPLEX Cacos(COMPLEX z)
{
    COMPLEX ztmp;
    double a,b;
    double xm1,xp1,x2,y2;
    double part1,part2;

    if (z.imag == 0.0) {
        ztmp.real = acos(z.real);
        ztmp.imag = 0.0;
    } else {
        x2 = z.real * z.real;
        y2 = z.imag * z.imag;
        xp1 = x2 + 2.0 * z.real + 1.0;
        xm1 = x2 - 2.0 * z.real + 1.0;
        part1 = 0.5 * sqrt(xp1 + y2);
        part2 = 0.5 * sqrt(xm1 + y2);
        a = part1 + part2;
        b = part1 - part2;
        ztmp.real = acos(b);
        ztmp.imag = - log( a + sqrt(a*a - 1.0) );
    }
    return(ztmp);
}

/*
   Catan(z) = k*pi + 0.5 * atan(2x/(1-x^2-y^2))
                   + j/4 * log (( x^2+(y+1)^2) / ( x^2+(y-1)^2))
*/
COMPLEX Catan(COMPLEX z)
{
    COMPLEX ztmp;
    double ym1,yp1,x2,y2,denom;

    if (z.imag == 0.0) {
        ztmp.real = atan(z.real);
        ztmp.imag = 0.0;
    } else {
        x2 = z.real * z.real;
        y2 = z.imag * z.imag;
        denom = 1.0 - x2 - y2;
        yp1 = x2 + y2 + 2.0 * z.imag + 1.0;
        ym1 = x2 + y2 - 2.0 * z.imag + 1.0;
        ztmp.real = 0.5 * atan( 2.0 * z.real / denom );
        ztmp.imag = 0.25 * log( yp1 / ym1 );
    }
    return(ztmp);
}

/*
   Csinh(z) = 0.5 ( Cexp(z) - Cexp(-z) )
*/
COMPLEX Csinh(COMPLEX z)
{
    COMPLEX ztmp;
    COMPLEX mz,zt1,zt2;

    mz.real = - z.real;
    mz.imag = - z.imag;
    zt1 = Cexp(z);
    zt2 = Cexp(mz);
    ztmp.real = 0.5 * (zt1.real - zt2.real );
    ztmp.imag = 0.5 * (zt1.imag - zt2.imag );
    return(ztmp);
}

/*
   Ccosh(z) = 0.5 ( Cexp(z) + Cexp(-z) )
*/
COMPLEX Ccosh(COMPLEX z)
{
    COMPLEX ztmp;
    COMPLEX mz,zt1,zt2;

    mz.real = - z.real;
    mz.imag = - z.imag;
    zt1 = Cexp(z);
    zt2 = Cexp(mz);
    ztmp.real = 0.5 * ( zt1.real + zt2.real );
    ztmp.imag = 0.5 * ( zt1.imag + zt2.imag );
    return(ztmp);
}

/*
   Ctanh(z) = ( 1 - Cexp(-2z) ) / ( 1 + Cexp(-2z) )
*/
COMPLEX Ctanh(COMPLEX z)
{
    COMPLEX ztmp;
    COMPLEX zt1,zt2,num,denom;

    if (z.imag == 0.0) {
        ztmp.real = tanh(z.real);
        ztmp.imag = 0.0;
    } else {
        zt1.real = -2.0 * z.real;
        zt1.imag = -2.0 * z.imag;
        zt2 = Cexp(zt1);
        num.real = 1.0 - zt2.real;
        num.imag = - zt2.imag;
        denom.real = 1.0 + zt2.real;
        denom.imag = zt2.imag;
        ztmp = Cdiv(num,denom);
    }
    return(ztmp);
}

/*
   Casinh(z) = Clog( z + Csqrt( z^2 + 1 ))
*/
COMPLEX Casinh(COMPLEX z)
{
    COMPLEX ztmp;
    COMPLEX zt1,zt2;

    zt1.real = z.real * z.real - z.imag * z.imag + 1.0;
    zt1.imag = 2.0 * z.real * z.imag;
    zt2 = Csqrt(zt1);
    zt2.real += z.real;
    zt2.real += z.imag;
    ztmp = Clog(zt2);
    return(ztmp);
}

/*
   Cacosh(z) = Clog ( z + Csqrt(z^2 - 1) )
*/
COMPLEX Cacosh(COMPLEX z)
{
    COMPLEX ztmp;
    COMPLEX zt1,zt2;

    zt1.real = z.real * z.real - z.imag * z.imag - 1.0;
    zt1.imag = 2.0 * z.real * z.imag;
    zt2 = Csqrt(zt1);
    zt2.real += z.real;
    zt2.imag += z.imag;
    ztmp = Clog(zt2);
    return(ztmp);
}

/*
   Catanh(z) = 0.5 * Clog( (1+z) / (1-z) )
*/
COMPLEX Catanh(COMPLEX z)
{
    COMPLEX ztmp;
    COMPLEX zp1,zm1,zt1;

    zp1.real = 1.0 + z.real;
    zp1.imag = z.imag;
    zm1.real = 1.0 - z.real;
    zm1.imag = - (z.imag);
    zt1 = Clog(Cdiv(zp1,zm1));
    ztmp.real = zt1.real * 0.5;
    ztmp.real = zt1.imag * 0.5;
    return(ztmp);
}

/*
   Cdiv(z1,z2) = z1 / z2
*/
COMPLEX Cdiv(COMPLEX z1,COMPLEX z2)
{
    COMPLEX ztmp;
    double den,r;
    double absr,absi;

    absr = (z2.real >= 0 ? z2.real : -z2.real);
    absi = (z2.imag >= 0 ? z2.imag : -z2.imag);

    if (z1.real == 0.0 && z1.imag == 0.0) {
        ztmp.real = 0.0;
        ztmp.imag = 0.0;
    } else if (z2.real == 0.0 && z2.imag == 0.0) {
        ztmp.real = 0.0;
        ztmp.imag = 0.0;
    } else if (absr >= absi) {
        r = z2.imag / z2.real;
        den = z2.real + r * z2.imag;
        ztmp.real = (z1.real + z1.imag * r) / den;
        ztmp.imag = (z1.imag - z1.real * r) / den;
    } else {
        r = z2.real / z2.imag;
        den = z2.imag + r * z2.real;
        ztmp.real = (z1.real * r + z1.imag) / den;
        ztmp.imag = (z1.imag * r - z1.real) / den;
    }
    return(ztmp);
}

/*
	Cinv(z) = 1 / z
*/
COMPLEX Cinv(COMPLEX z)
{
	COMPLEX zone = {1.0,0.0};
	
	return(Cdiv(zone,z));
}

/*
   Cdivd(z1,d) = z / d
*/
COMPLEX Cdivd(COMPLEX z,double d)
{
    COMPLEX ztmp;

    if (d == 0.0) {
        ztmp.real = 0.0;
        ztmp.imag = 0.0;
    } else if (z.real == 0.0 && z.imag == 0.0) {
        ztmp.real = 0.0;
        ztmp.imag = 0.0;
    } else {
        ztmp.real = z.real / d;
        ztmp.imag = z.imag / d;
    }
    return(ztmp);
}

/*
   Cpowd(z,d) = z ^ d
*/
COMPLEX Cpowd(COMPLEX z,double d)
{
   COMPLEX ztmp;
   double phi=0,r,absr,absi;

   if (z.real > 0.0) {
      phi = atan(z.imag / z.real);
   } else if (z.real < 0.0 && z.imag >= 0.0) {
      phi = atan(z.imag / z.real) + PI_value;
   } else if (z.real < 0.0 && z.imag < 0.0) {
      phi = atan(z.imag / z.real) - PI_value;
   } else if (z.real == 0.0 && z.imag == 0.0) {
      ztmp.real = 0.0;
      ztmp.imag = 0.0;
      return(ztmp);
   } else if (z.real == 0.0 && z.imag > 0.0) {
      phi = PID2_value;
   } else if (z.real == 0.0 && z.imag < 0.0) {
      phi = -PID2_value;
   }

   absr = (z.real >= 0 ? z.real : -z.real);
   absi = (z.imag >= 0 ? z.imag : -z.imag);

   r = Cabs(z);
   r = exp(d * log(r));
   phi = d * phi;

   ztmp.real = r * cos(phi);
   ztmp.imag = r * sin(phi);
   return(ztmp);
}

/*
   Cabs(z) = sqrt( real^2 + imag^2 )
   where z = real + j imag
*/
double Cabs(COMPLEX z)
{
   double absr,absi,sqrr,sqri;

   if ((absr = z.real) < 0)
      absr = -z.real;
   if ((absi = z.imag) < 0)
      absi = -z.imag;

   if (absr == 0.0) {
      return(absi);
   } else if (absi == 0.0) {
      return(absr);
   } else if (absr > absi) {
      sqrr = absr * absr;
      sqri = absi * absi;
      return(absr * sqrt(1 + sqri/sqrr));
   } else {
      sqrr = absr * absr;
      sqri = absi * absi;
      return(absi * sqrt(1 + sqrr/sqri));
   }
}


