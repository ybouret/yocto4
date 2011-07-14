#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto {
	
	namespace math {
		
		template <>
		bool bracket<real_t>::inside(numeric<real_t>::function    & func,
									 triplet<real_t>              & x,
									 triplet<real_t>              & f )
		{
			static const real_t fmax = Sqrt(numeric<real_t>::ftol);     //!< numeric constraint
			//==================================================================
			//
			// try to find x.b so that f.b <= f.a and f.b <= f.c
			//
			//==================================================================
			//std::cout << "bracketing in [" << x.a << " ; " << x.c << "]" << std::endl;
			
			while( Fabs(x.c-x.a) > fmax * ( Fabs(x.a) + Fabs(x.c) ))
			{
				x.b = REAL(0.5)*(x.a + x.c);
				f.b = func(x.b);
				
				//std::cout << "x=[ " << x.a << " " << x.b << " " << x.c << " ]" << std::endl;
				//std::cout << "f=[ " << f.a << " " << f.b << " " << f.c << " ]" << std::endl;
				
				if( f.b > f.a )
				{
					//----------------------------------------------------------
					// a bump is detected between a and c
					// move c at this bump !
					//----------------------------------------------------------
					x.c = x.b;
					f.c = f.b;
					continue;
				}
				else {
					//-- going to b will decrease the function
					assert( f.b <= f.a );
					if( f.b > f.c )
					{
						//------------------------------------------------------
						// the minimum lies between b and c
						// move a at b
						//------------------------------------------------------
						x.a = x.b;
						f.a = f.b;
						continue;
					}
					else
					{
						//-- success !
						assert( f.b <= f.c );
						assert( f.b <= f.a );
						assert( (x.a<=x.b&& x.b<=x.c) || ( x.a>=x.b && x.b>=x.c) );
						//std::cout << "success!" << std::endl;
						//exit(1);
						return true;
					}
				}
				
			}
			
			//==================================================================
			// no convergence=> one side is a minimum
			// prepare the triplets so that minimize will work !
			//==================================================================
			if( f.a < f.c )
			{
				x.b = x.c = x.a;
				f.b = f.c = f.a;
			}
			else
			{
				x.b = x.a = x.c;
				f.b = f.a = f.c;
			}
			assert( f.b <= f.c );
			assert( f.b <= f.a );
			assert( (x.a<=x.b && x.b<=x.c) || ( x.a>=x.b && x.b>=x.c) );
			return false;
		}
		
		
		
	} //math
	
}//yocto
