#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"

//#include <iostream>

namespace yocto {
	
	namespace math {
		
		
		
		
#define SHIFT3(a,b,c,d) do { (a)=(b);(b)=(c);(c)=(d); } while(false)
		
		
		template <>
		void minimize<real_t>( numeric<real_t>::function &func, triplet<real_t> &x, triplet<real_t> &f, real_t ftol )
		{
			assert(f.b<=f.a);
			assert(f.b<=f.c);
			assert(x.is_ordered());
			
			
			static const real_t fmax = Sqrt(numeric<real_t>::ftol); //!< numeric constraint
			static const real_t dmax = numeric<real_t>::tiny;       //!< numeric constraint
			static const real_t R    = REAL(0.618034);
			static const real_t C    = REAL(1.0)-R;
			
			
			const real_t xtol = max_of<real_t>( fmax, Fabs(ftol) );
			
			real_t       x0   = x.a, x1, x2, x3 = x.c;
			real_t       f0   = f.a, f1, f2, f3 = f.c;
			
			if( Fabs( x.c - x.b ) > Fabs( x.b - x.a ) )
			{
				x1 = x.b;
				f1 = f.b;
				x2 = x.b + C * ( x.c- x.b );
				f2 = func( x2 );
			}
			else
            {
				x2 = x.b;
				f2 = f.b;
				x1 = x.b - C * (x.b - x.a );
				f1 = func( x1 );
			}
			
			while(Fabs( x3 - x0 ) > max_of<real_t>(xtol * ( Fabs(x1) + Fabs(x2) ), dmax) )
			{
				
				if( f2 < f1 )
				{
					SHIFT3(x0,x1,x2,R*x1+C*x3);
					SHIFT3(f0,f1,f2,func(x2));
				}
				else
				{
					SHIFT3(x3,x2,x1,R*x2+C*x0);
					SHIFT3(f3,f2,f1,func(x1));
				}
				//std::cerr << "x=[ " << x0 << " " << x1 << " " << x2 << " " << x3 << " ]" << std::endl;
                //std::cerr << "f=[ " << f0 << " " << f1 << " " << f2 << " " << f2 << " ]" << std::endl;
				//std::cerr << "del=" << Fabs( x3 - x0 ) << " | brk=" << xtol * ( Fabs(x1) + Fabs(x2) ) << " (xtol=" << xtol << ")" << std::endl;
			}
			
			//std::cerr << "x=[ " << x0 << " " << x1 << " " << x2 << " " << x3 << " ]" << std::endl;
			//std::cerr << "f=[ " << f0 << " " << f1 << " " << f2 << " " << f3 << " ]" << std::endl;
			
			if( f1 <= f2 )
			{
				x.a = x0; f.a = f0;
				x.b = x1; f.b = f1;
				x.c = x2; f.c = f2;
			}
			else
			{
				x.a = x1; f.a = f1;
				x.b = x2; f.b = f2;
				x.c = x3; f.c = f3;
			}
			
		}
		
		
		
		
	} // math
	
} //yocto
