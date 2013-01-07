#include "./rand32.hpp"
#include <cstdio>

void Rand32::test(void) throw()
{
    settable(12345,65435,34221,12345,9983651,95746118);
    uint32_t k;
    int      i;
    for(i=1;i<1000001;i++){k=lfib4();} printf("%u\n", k-1064612766U);
    for(i=1;i<1000001;i++){k=swb()  ;} printf("%u\n", k- 627749721U);
    for(i=1;i<1000001;i++){k=kiss() ;} printf("%u\n", k-1372460312U);
    for(i=1;i<1000001;i++){k=cong() ;} printf("%u\n", k-1529210297U);
    for(i=1;i<1000001;i++){k=shr3() ;} printf("%u\n", k-2642725982U);
    for(i=1;i<1000001;i++){k=mwc()  ;} printf("%u\n", k- 904977562U);
    for(i=1;i<1000001;i++){k=fib()  ;} printf("%u\n", k-3519793928U);
}


#define znew   (z=36969*(z&65535)+(z>>16))
#define wnew   (w=18000*(w&65535)+(w>>16))
#define MWC    ((znew<<16)+wnew )
#define SHR3  (jsr^=(jsr<<17), jsr^=(jsr>>13), jsr^=(jsr<<5))
#define CONG  (jcong=69069*jcong+1234567)
#define FIB   ((b=a+b),(a=b-a))
#define KISS  ((MWC^CONG)+SHR3)
#define LFIB4 (c++,t[c]=t[c]+t[UC(c+58)]+t[UC(c+119)]+t[UC(c+178)])
#define SWB   (c++,bro=(x<y),t[c]=(x=t[UC(c+34)])-(y=t[UC(c+19)]+bro))
#define UNI   (KISS*2.328306e-10)
#define VNI   ((long) KISS)*4.656613e-10
#define UC    (uint8_t)  /*a cast operation*/

#if defined(_MSC_VER)
// initialisation of t
#pragma warning ( disable : 4351 )
#endif

Rand32:: Rand32() throw() :
z(362436069),
w(521288629),
jsr(123456789),
jcong(380116160),
a(224466889),
b(7584631),
x(0),
y(0),
bro(0),
c(0),
t()
{
    for(size_t i=0;i<256;++i)
        t[i]=KISS;
}

void Rand32:: settable( uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t i5, uint32_t i6 ) throw()
{
    z=i1;w=i2,jsr=i3; jcong=i4; a=i5; b=i6;
    for(size_t i=0;i<256;++i)
        t[i]=KISS;
    c=0;
}


void Rand32:: reset( uint32_t s ) throw()
{
    uint32_t iv[6];
    iv[0] = IntegerHash32(s);
    for( size_t i=1; i < 6; ++i )
        iv[i] = IntegerHash32( iv[i-1] );
    settable(iv[0], iv[1], iv[2], iv[3], iv[4], iv[5]);
}

Rand32:: ~Rand32() throw()
{
}

uint32_t Rand32:: mwc() throw()
{
    return MWC;
}

uint32_t Rand32:: shr3() throw()
{
    return SHR3;
}

uint32_t Rand32:: cong() throw()
{
    return CONG;
}

uint32_t Rand32:: fib() throw()
{
    return FIB;
}

uint32_t Rand32:: kiss() throw()
{
    return KISS;
}

uint32_t Rand32:: lfib4() throw()
{
    return LFIB4;
}

uint32_t Rand32:: swb() throw()
{
    return SWB;
}

UniformGenerator::  UniformGenerator() throw() {}
UniformGenerator:: ~UniformGenerator() throw() {}



template <>
uint32_t UniformGenerator::get<uint32_t>() throw()
{
    return next();
}

template <>
double UniformGenerator::get<double>() throw()
{
    return Rand32::ToDouble( next() );
}

template <>
float UniformGenerator::get<float>() throw()
{
    return Rand32::ToFloat( next() );
}

#include "./sort.hpp"
void UniformGenerator:: fill_array( double a, double b, double *x, size_t n ) throw()
{
    assert(!(x==NULL&&n>0));
    const double l = b-a;
    for( size_t i=0; i < n; ++i )
    {
        x[i] = a + alea() * l;
    }
    Sort(x,n);
}
