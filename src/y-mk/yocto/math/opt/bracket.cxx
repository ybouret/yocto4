#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/swap.hpp"
#include "yocto/code/utils.hpp"

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
		
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d)
        
		template <>
        bool bracket<real_t>::expand( numeric<real_t>::function &func, triplet<real_t> &x, triplet<real_t> &f )
        {
            static const real_t GOLD = 1.618034;
            static const real_t GLIM = 10;
            static const real_t TINY = 1e-20;
            
            //------------------------------------------------------------------
            // assume f.a and f.b are computed
            //------------------------------------------------------------------
            if (f.b > f.a)
            {
                cswap(x.a,x.b);
                cswap(f.a,f.b);
            }
            
            //------------------------------------------------------------------
            // now, we go downhill: initialize third point
            //-----------------------------------------------------------------
            x.c = x.b + GOLD * (x.b - x.a);
            f.c = func(x.c);
            while( f.b > f.c )
            {
                assert(f.b<= f.a);
                assert( (x.a<=x.b && x.b <= x.c) || ( x.a >= x.b && x.b >= x.c));
                const real_t width = x.c - x.a;
                
                //-- interval is too small
                if( Fabs(width) <= 0 )
                    return false;
                
                //-- points are not well set
                const real_t beta  = (x.b - x.c) / width;
                if( beta <=0 || beta >=1 )
                    return false;
                
                const real_t dc = f.c - f.a;
                const real_t db = f.b - f.a;
                const real_t q  = db - beta * dc;
                
                if(q>0)
                {
                    //----------------------------------------------------------
                    // possible parabolic fit
                    //----------------------------------------------------------
                    const real_t p    = beta * beta * dc - db;
                    const real_t lam  = -p/( 2*Signed(max_of(Fabs(q),TINY),q) );
                    const real_t u    = x.a + lam * width;
                    const real_t ulim = x.b + GLIM * (x.b - x.a);
                    const real_t fu   = func(u);
                    
                    //----------------------------------------------------------
                    // if u is between a and b
                    //----------------------------------------------------------
                    if( (u-x.a) * (x.b-u) >= 0 )
                    {
                     
                        continue;
                    }
                    
                    //----------------------------------------------------------
                    // if u is between b and c
                    //----------------------------------------------------------
                    if( (u-x.b) * (x.c-u) >= 0)
                    {
                        
                        continue;
                    }
                    
                    //----------------------------------------------------------
                    // if u is between c and ulim
                    //----------------------------------------------------------
                    if( (u-x.c) * (ulim -u ) >= 0)
                    {
                        
                        continue;
                    }
                    
                    //-- reject fit
                }
                
                //--------------------------------------------------------------
                // negative curvature or rejected fit: use default probe
                //--------------------------------------------------------------

                // a <-- b
                x.a = x.b;
                f.a = f.b;
                // b <-- c
                x.b = x.c;
                f.b = f.c;
                // c <-- forward
                x.c = x.b + GOLD * (x.b - x.a);
                f.c = func(x.c);

            }
            
            return true;
        }
        
		
	} //math
	
}//yocto
