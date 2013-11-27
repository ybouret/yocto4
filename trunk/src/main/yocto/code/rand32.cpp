#include "yocto/code/rand32.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/math/types.hpp"

#include <cstdio>

namespace yocto
{
    namespace core
    {
        
        
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
        
#if 0
        typedef uint32_t UL;
        /*  Global static variables: */
        static UL z=362436069, w=521288629, jsr=123456789, jcong=380116160;
        static UL a=224466889, b=7584631, t[256];
        /* Use random seeds to reset z,w,jsr,jcong,a,b, and the table t[256]*/
        static UL x=0,y=0,bro; static unsigned char c=0;
        /* Example procedure to set the table, using KISS: */
        void settable(UL i1,UL i2,UL i3,UL i4,UL i5, UL i6)
        {
            int i; z=i1;w=i2,jsr=i3; jcong=i4; a=i5; b=i6;
            for(i=0;i<256;i=i+1)  t[i]=KISS;
            c=0;
        }
        
        /* This is a test main program.  It should compile and print 7  0's. */
        void test_rand32(void){
            int i; UL k;
            settable(12345,65435,34221,12345,9983651,95746118);
            for(i=1;i<1000001;i++){k=LFIB4;} printf("%u\n", k-1064612766U);
            for(i=1;i<1000001;i++){k=SWB  ;} printf("%u\n", k- 627749721U);
            for(i=1;i<1000001;i++){k=KISS ;} printf("%u\n", k-1372460312U);
            for(i=1;i<1000001;i++){k=CONG ;} printf("%u\n", k-1529210297U);
            for(i=1;i<1000001;i++){k=SHR3 ;} printf("%u\n", k-2642725982U);
            for(i=1;i<1000001;i++){k=MWC  ;} printf("%u\n", k- 904977562U);
            for(i=1;i<1000001;i++){k=FIB  ;} printf("%u\n", k-3519793928U);
            settable(12345,65435,34221,12345,9983651,95746118);
        }
#else
        
        
        
        void rand32::test(void)
        {
            rand32 r;
            r.settable(12345,65435,34221,12345,9983651,95746118);
            uint32_t k;
            int      i;
            for(i=1;i<1000001;i++){k=r.lfib4();} printf("%u\n", k-1064612766U);
            for(i=1;i<1000001;i++){k=r.swb()  ;} printf("%u\n", k- 627749721U);
            for(i=1;i<1000001;i++){k=r.kiss() ;} printf("%u\n", k-1372460312U);
            for(i=1;i<1000001;i++){k=r.cong() ;} printf("%u\n", k-1529210297U);
            for(i=1;i<1000001;i++){k=r.shr3() ;} printf("%u\n", k-2642725982U);
            for(i=1;i<1000001;i++){k=r.mwc()  ;} printf("%u\n", k- 904977562U);
            for(i=1;i<1000001;i++){k=r.fib()  ;} printf("%u\n", k-3519793928U);
        }
        
#if defined(_MSC_VER)
		// init of t
#pragma warning ( disable : 4351 )
#endif
        rand32:: rand32() throw() :
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
        
        void rand32:: settable( uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t i5, uint32_t i6 ) throw()
        {
            z=i1;w=i2,jsr=i3; jcong=i4; a=i5; b=i6;
            for(size_t i=0;i<256;++i)
                t[i]=KISS;
            c=0;
        }
        
        static inline
        uint32_t __ih32( uint32_t a)
        {
            a = (a+0x7ed55d16) + (a<<12);
            a = (a^0xc761c23c) ^ (a>>19);
            a = (a+0x165667b1) + (a<<5);
            a = (a+0xd3a2646c) ^ (a<<9);
            a = (a+0xfd7046c5) + (a<<3);
            a = (a^0xb55a4f09) ^ (a>>16);
            return a;
        }
        
        void rand32:: reset( uint32_t s ) throw()
        {
            uint32_t iv[6];
            iv[0] = __ih32(s);
            for( size_t i=1; i < 6; ++i )
                iv[i] = __ih32( iv[i-1] );
            settable(iv[0], iv[1], iv[2], iv[3], iv[4], iv[5]);
        }
        
        rand32:: ~rand32() throw()
        {
        }
        
        uint32_t rand32:: mwc() throw()
        {
            return MWC;
        }
        
        uint32_t rand32:: shr3() throw()
        {
            return SHR3;
        }
        
        uint32_t rand32:: cong() throw()
        {
            return CONG;
        }
        
        uint32_t rand32:: fib() throw()
        {
            return FIB;
        }
        
        uint32_t rand32:: kiss() throw()
        {
            return KISS;
        }
        
        uint32_t rand32:: lfib4() throw()
        {
            return LFIB4;
        }
        
        uint32_t rand32:: swb() throw()
        {
            return SWB;
        }
        
#endif
    }
    
    urand32::  urand32() throw() {}
    urand32:: ~urand32() throw() {}
    
    
    
    template <>
    uint32_t urand32::get<uint32_t>() throw()
    {
        return next();
    }
    
    template <>
    double urand32::get<double>() throw()
    {
        return core::rand32::to_double( next() );
    }
    
    template <>
    float urand32::get<float>() throw()
    {
        return core::rand32::to_float( next() );
    }
    
    void urand32:: wseed() throw()
    {
        seed( wtime::seed() );
    }
    
    void urand32:: gaussian(double &u, double &v) throw()
    {
        static const double two_pi = math::numeric<double>::two_pi;
        double         x1    = 0;
        while( x1<=0 ) x1  = get<double>();
        const double x2    = get<double>();
        const double l     = math::Log( x1 );
        const double theta = two_pi * x2;
        const double rho   = math::Sqrt( -(l+l) );
        
        u = rho * sin( theta );
        v = rho * cos( theta );
    }

    void urand32:: hypersphere(const double radius, double arr[], const size_t num) throw()
    {
        assert(num>0);
        assert(arr!=0);
        double fac = 0;
        for(size_t i=0;;)
        {
            double u=0,v=0;
            gaussian(u, v);
            arr[i++] = u; fac += u*u;
            if(i>=num) break;
            arr[i++] = v; fac += v*v;
            if(i>=num) break;
        }
        fac = radius/math::Sqrt(fac);
        for(size_t i=0;i<num;++i)
        {
            arr[i] *= fac;
        }
    }
    

}
/*-----------------------------------------------------
 Write your own calling program and try one or more of
 the above, singly or in combination, when you run a
 simulation. You may want to change the simple 1-letter
 names, to avoid conflict with your own choices.        */
/* All that follows is comment, mostly from the initial
 post. You may want to remove it */
/* Any one of KISS, MWC, FIB, LFIB4, SWB, SHR3, or CONG
 can be used in an expression to provide a random 32-bit
 integer.
 The KISS generator, (Keep It Simple Stupid), is
 designed to combine the two multiply-with-carry
 generators in MWC with the 3-shift register SHR3 and
 the congruential generator CONG, using addition and
 exclusive-or. Period about 2^123.
 It is one of my favorite generators.
 The  MWC generator concatenates two 16-bit multiply-
 with-carry generators, x(n)=36969x(n-1)+carry,
 y(n)=18000y(n-1)+carry  mod 2^16, has period about
 2^60 and seems to pass all tests of randomness. A
 favorite stand-alone generator---faster than KISS,
 which contains it.
 FIB is the classical Fibonacci sequence
 x(n)=x(n-1)+x(n-2),but taken modulo 2^32.
 Its period is 3*2^31 if one of its two seeds is odd
 and not 1 mod 8. It has little worth as a RNG by
 itself, but provides a simple and fast component for
 use in combination generators.
 SHR3 is a 3-shift-register generator with period
 2^32-1. It uses y(n)=y(n-1)(I+L^17)(I+R^13)(I+L^5),
 with the y's viewed as binary vectors, L the 32x32
 binary matrix that shifts a vector left 1, and R its
 transpose.  SHR3 seems to pass all except those
 related to the binary rank test, since 32 successive
 values, as binary vectors, must be linearly
 independent, while 32 successive truly random 32-bit
 integers, viewed as binary vectors, will be linearly
 independent only about 29% of the time.
 CONG is a congruential generator with the widely used 69069
 multiplier: x(n)=69069x(n-1)+1234567.  It has period
 2^32. The leading half of its 32 bits seem to pass
 tests, but bits in the last half are too regular.
 LFIB4 is an extension of what I have previously
 defined as a lagged Fibonacci generator:
 x(n)=x(n-r) op x(n-s), with the x's in a finite
 set over which there is a binary operation op, such
 as +,- on integers mod 2^32, * on odd such integers,
 exclusive-or(xor) on binary vectors. Except for
 those using multiplication, lagged Fibonacci
 generators fail various tests of randomness, unless
 the lags are very long. (See SWB below).
 To see if more than two lags would serve to overcome
 the problems of 2-lag generators using +,- or xor, I
 have developed the 4-lag generator LFIB4 using
 addition: x(n)=x(n-256)+x(n-179)+x(n-119)+x(n-55)
 mod 2^32. Its period is 2^31*(2^256-1), about 2^287,
 and it seems to pass all tests---in particular,
 those of the kind for which 2-lag generators using
 +,-,xor seem to fail.  For even more confidence in
 its suitability,  LFIB4 can be combined with KISS,
 with a resulting period of about 2^410: just use
 (KISS+LFIB4) in any C expression.
 SWB is a subtract-with-borrow generator that I
 developed to give a simple method for producing
 extremely long periods:
 x(n)=x(n-222)-x(n-237)- borrow mod 2^32.
 The 'borrow' is 0, or set to 1 if computing x(n-1)
 caused overflow in 32-bit integer arithmetic. This
 generator has a very long period, 2^7098(2^480-1),
 about 2^7578.   It seems to pass all tests of
 randomness, except for the Birthday Spacings test,
 which it fails badly, as do all lagged Fibonacci
 generators using +,- or xor. I would suggest
 combining SWB */
