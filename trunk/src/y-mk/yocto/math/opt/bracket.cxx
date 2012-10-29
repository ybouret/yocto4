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
			
			while( Fabs(x.c-x.a) > fmax * ( Fabs(x.a) + Fabs(x.c) ))
			{
				x.b = REAL(0.5)*(x.a + x.c);
				f.b = func(x.b);
				
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
		
#define SHFT(a,b,c,d) do{ (a)=(b);(b)=(c);(c)=(d); } while(false)
        
		template <>
        bool bracket<real_t>::expand( numeric<real_t>::function &func, triplet<real_t> &x, triplet<real_t> &f )
        {
            static const real_t GOLD = 1.618034;
            static const real_t GLIM = 4;
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
            // now, we go downhill: initialize the third point
            //-----------------------------------------------------------------
            x.c = x.b + GOLD * (x.b - x.a);
            f.c = func(x.c);
            std::cerr << "x=" << x << std::endl;
            std::cerr << "f=" << f << std::endl;
            
            while( f.b > f.c )
            {
                std::cerr << "\tx=" << x << std::endl;
                std::cerr << "\tf=" << f << std::endl;
                assert(f.b<= f.a);
                assert( (x.a<=x.b && x.b <= x.c) || ( x.a >= x.b && x.b >= x.c));
                const real_t delta = x.c - x.a;
                
                std::cerr << "\tdelta=" << delta << std::endl;
                //-- interval is too small
                if( Fabs(delta) <= 0 )
                    return false;
                
                //-- points are not well set
                const real_t beta  = (x.b - x.a) / delta;
                std::cerr << "\tbeta=" << beta << std::endl;
                if( beta <=0 || beta >=1 )
                    return false;
                
                const real_t dc = f.c - f.a;
                const real_t db = f.b - f.a;
                const real_t q  = beta * dc - db;
                
                std::cerr << "\tq=" << q << std::endl;
                if(q>0)
                {
                    //----------------------------------------------------------
                    // possible parabolic fit
                    //----------------------------------------------------------
                    const real_t p    = db - beta * beta * dc;                   assert(p<0);
                    const real_t lam  = -p/( 2*Signed(max_of(Fabs(q),TINY),q) ); assert(lam>0);
                    const real_t u    = x.a + lam * delta;
                    const real_t ulim = x.b + GLIM * (x.b - x.a);
                    
                    std::cerr << "\tlam =" << lam << std::endl;
                    std::cerr << "\tu   =" << u << std::endl;
                    std::cerr << "\tulim=" << ulim << std::endl;
                    
                    //----------------------------------------------------------
                    // if u is between a and b
                    //----------------------------------------------------------
                    if( (u-x.a) * (x.b-u) >= 0 )
                    {
                        
                        std::cerr << "\t\tbetween a and b" << std::endl;
                        return false;
                        continue;
                    }
                    
                    //----------------------------------------------------------
                    // if u is between b and c
                    //----------------------------------------------------------
                    if( (u-x.b) * (x.c-u) >= 0)
                    {
                        std::cerr << "\t\tbetween b and c" << std::endl;
                        return false;
                        continue;
                    }
                    
                    //----------------------------------------------------------
                    // if u is between c and ulim: move to b,c,u
                    //----------------------------------------------------------
                    if( (u-x.c) * (ulim -u ) >= 0)
                    {
                        const real_t fu   = func(u);
                        SHFT(x.a,x.b,x.c,u);
                        SHFT(f.a,f.b,f.c,fu);
                        std::cerr << "\t\tbetween c and ulim" << std::endl;
                        continue;
                    }
                    
                    //----------------------------------------------------------
                    //-- reject fit: accept ulim
                    //----------------------------------------------------------
                    std::cerr << "\t\taccept ulim" << std::endl;
                    SHFT(x.a,x.b,x.c,ulim);
                    SHFT(f.a,f.b,f.c,func(ulim));
                    continue;
                    
                }
                
                //--------------------------------------------------------------
                // negative curvature
                //--------------------------------------------------------------
                std::cerr << "\tdefault step" << std::endl;
                SHFT(x.a, x.b, x.c, x.b + GOLD * (x.b - x.a) );
                SHFT(f.a,f.b,f.c,func(x.c));
            }
            std::cerr << "x=" << x << std::endl;
            std::cerr << "f=" << f << std::endl;
            return true;
        }
        
		
	} //math
	
}//yocto
