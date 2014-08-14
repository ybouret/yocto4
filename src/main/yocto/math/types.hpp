#ifndef YOCTO_MATH_TYPES_INCLUDED
#define YOCTO_MATH_TYPES_INCLUDED 1

#include "yocto/math/complex.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/functor.hpp"

#include <cmath>

namespace yocto
{
	namespace math
	{
		template <typename T>
		struct real_of;
		
		template <>
		struct real_of<float>
		{
			typedef float type;
		};
		
		template <>
		struct real_of<double>
		{
			typedef double type;
		};
		
		
		template <>
		struct real_of< complex<float> >
		{
			typedef float type;
		};
		
		template <>
		struct real_of< complex<double> >
		{
			typedef double type;
		};
		
		
		template <typename T>
		struct numeric
		{
			static const T      epsilon;
			static const T      sqrt_epsilon;
			static const T      minimum;
			static const T      maximum;
			static const T      pi;
			static const T      two_pi;
			static const size_t dig;
			static const T      ftol;
			static const T      min_exp;
			static const T      max_exp;
			static const T      min_10_exp;
			static const T      max_10_exp;
			static const T      tiny;      //!< 10^{min_10_exp}
			static const T      huge;      //!< 10^{max_10_exp}
			static const T      sqrt_tiny;
            
			typedef functor<T,TL1(T)>                            function;
            typedef functor<T,TL2(T,T)>                          function2;

			typedef functor<T,TL1(const array<T>&)>              scalar_field;
			typedef functor<void,TL2(array<T>&,const array<T>&)> vector_field;
            typedef T                                           (*CFunction)(T);
            typedef T                                           (*CFunction2)(T,T);

            static const T zero;
            static const T one;
            static const T two;
            
            static void round_error( T &x ) throw();
		};
		
        
		
		inline float  Fabs( float x  ) throw() { return fabsf( x ); }
		inline double Fabs( double x ) throw() { return fabs( x );  }
		
        template <typename T, typename U>
        inline T Signed( T a, U b) throw()
        {
            return b >= 0 ? Fabs(a) : -Fabs(a);
        }
        
		inline float  Square( float  x ) throw() { return x*x; }
		inline double Square( double x ) throw() { return x*x; }
		inline float  Square( const complex<float>  &x ) throw() { return x.re * x.re + x.im * x.im; }
		inline double Square( const complex<double> &x ) throw() { return x.re * x.re + x.im * x.im; }
		
		
		inline float  Sqrt( float  x ) throw()  { return sqrtf( x ); }
		inline double Sqrt( double x ) throw()  { return sqrt(  x );  }
		
		inline float  Fabs( const complex<float>  &x ) throw() { return Sqrt( x.re * x.re + x.im * x.im ); }
		inline double Fabs( const complex<double> &x ) throw() { return Sqrt( x.re * x.re + x.im * x.im ); }
		
        
		inline float  Cos( float  x ) throw()  { return cosf( x ); }
		inline double Cos( double x ) throw()  { return cos(  x );  }
        
        inline float  Acos( float x) throw()   { return acosf(x); }
        inline double Acos( double x ) throw() { return acos(x); }
		
        inline float  Sin( float  x ) throw()  { return sinf( x ); }
		inline double Sin( double x ) throw()  { return sin(  x );  }
        
        inline float  Asin( float  x ) throw()  { return asinf( x ); }
		inline double Asin( double x ) throw()  { return asin(  x );  }
		
		inline float  Tan( float  x ) throw()  { return tanf( x ); }
		inline double Tan( double x ) throw()  { return tan(  x );  }
		
        inline float  Atan( float  x ) throw()  { return atanf( x ); }
		inline double Atan( double x ) throw()  { return atan(  x );  }
        
        float  Atan2( float x, float y ) throw();    //! in ]-\pi,\pi]
        double Atan2( double x, double y ) throw();  //! in ]-\pi,\pi]
        
        
		inline float  Pow( float  x, float  y ) throw() { return powf(x,y); }
		inline double Pow( double x, double y ) throw() { return pow(x,y);  }
        
		
		
		inline float  Exp( float  x ) throw()  { return expf( x ); }
		inline double Exp( double x ) throw()  { return exp(  x );  }
		
		inline float  Log( float  x ) throw()  { return logf( x ); }
		inline double Log( double x ) throw()  { return log(  x );  }
		
		inline float  Log10( float  x ) throw()  { return log10f( x ); }
		inline double Log10( double x ) throw()  { return log10(  x );  }
		
		inline float  Ceil( float  x ) throw()  { return ceilf( x ); }
		inline double Ceil( double x ) throw()  { return ceil(  x );  }
		
		inline float  Floor( float  x ) throw()  { return floorf( x ); }
		inline double Floor( double x ) throw()  { return floor(  x );  }
		
        
		inline float Hypotenuse( float a, float b) throw()
        {
            const float absa=Fabs(a);
            const float absb=Fabs(b);
            if (absa > absb)
                return absa*Sqrt(1.0f+Square(absb/absa));
            else
                return (absb <= 0.0f ? 0.0f : absb*Sqrt(1.0f+Square(absa/absb)));
        }
        
        inline double Hypotenuse( double a, double b) throw()
        {
            const double absa=Fabs(a);
            const double absb=Fabs(b);
            if (absa > absb)
                return absa*Sqrt(1.0+Square(absb/absa));
            else
                return (absb <= 0.0 ? 0.0 : absb*Sqrt(1.0+Square(absa/absb)));
        }
        
        template <typename T>
        inline int CompareModules( const T &x, const T &y ) throw()
        {
            typename real_of<T>::type a = Fabs(x), b = Fabs(y);
            return (a<b) ? -1 : ( (b<a) ? 1 : 0 );
        }
        
	}
	
}

#endif
