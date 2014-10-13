#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/bswap.hpp"
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
        void bracket<real_t>::expand( numeric<real_t>::function &func, triplet<real_t> &x, triplet<real_t> &f )
        {
            static const real_t GOLD = REAL(1.618034);
            static const real_t GLIM = REAL(10.0);
            static const real_t TINY = REAL(1e-20);
            
            //------------------------------------------------------------------
            // assume f.a and f.b are computed
            //------------------------------------------------------------------
            if (f.b > f.a)
            {
                cswap(x.a,x.b);
                cswap(f.a,f.b);
            }
            assert(f.b<=f.a);
            
            //------------------------------------------------------------------
            // now, we go downhill: initialize the third point
            //-----------------------------------------------------------------
            x.c = x.b + GOLD * (x.b - x.a);
            f.c = func(x.c);
            //std::cerr << "xinit:  " << x << std::endl;
            //std::cerr << "finit:  " << f << std::endl;
            
            while( f.b > f.c )
            {
                assert(f.b<=f.a);
                assert(f.c<f.b);
                assert(x.is_ordered());
                
                const real_t delta = x.c - x.a;
                if( Fabs(delta) <= 0 )
                {
                    f.a = f.b = f.c;
                    x.a = x.b = x.c;
                    return;
                }
                
                //--------------------------------------------------------------
                // compute geometrical factors
                //--------------------------------------------------------------
                const real_t alpha = (x.b - x.a) / delta; assert(alpha>=0);
                const real_t beta  = (x.c - x.b) / delta; assert(beta>=0);
                const real_t A     =  f.a - f.b;          assert(A>=0);
                const real_t B     =  f.b - f.c;          assert(B>0);
                
                //--------------------------------------------------------------
                // compute curvature times determinant>0
                //--------------------------------------------------------------
                const real_t q     = beta * A - alpha * B;
                
                if( q > 0 )
                {
                    
                    //----------------------------------------------------------
                    // compute the probe and its limit
                    //----------------------------------------------------------
                    const real_t p    = alpha*alpha*B + beta*beta*A; assert(p>=0);
                    const real_t lam  = p / ( 2 * max_of(q,TINY) );  assert(lam>=0);
                    const real_t u    = x.b + lam * delta;
                    const real_t ulim = x.b + GLIM * (x.c - x.b);
                    
                    
                    if( (u-x.b)*(x.c-u) >= 0 )
                    {
                        //------------------------------------------------------
                        // parabolic fit between between b and c
                        //------------------------------------------------------
                        const real_t fu = func(u);
                        if( fu <= f.c )
                        {
                            x.a = x.b; x.b = u;
                            f.a = f.b; f.b = fu;
                            assert(f.b<=f.a);
                            assert(x.is_ordered());
                            continue; // winner
                        }
                        
                        if( fu >= f.b )
                        {
                            x.c = u;
                            f.c = u;
                            assert(f.b<=f.a);
                            assert(x.is_ordered());
                            continue; // winner
                        }
                        
                        goto PROBE; // no interest
                    }
                    
                    if( (u-x.c) * (ulim -u ) >= 0 )
                    {
                        //----------------------------------------------------------
                        // between c and ulim
                        //----------------------------------------------------------
                        const real_t fu   = func(u);
                        SHFT(x.a,x.b,x.c,u);
                        SHFT(f.a,f.b,f.c,fu);
                        assert(f.b<=f.a);
                        assert(x.is_ordered());
                        continue;
                    }
                    
                    //----------------------------------------------------------
                    // beyond ulim
                    //----------------------------------------------------------
                    SHFT(x.a,x.b,x.c,ulim);
                    SHFT(f.a,f.b,f.c,func(ulim));
                    assert(f.b<=f.a);
                    assert(x.is_ordered());
                    continue;
                }
                
                //--------------------------------------------------------------
                // default magnification step
                //--------------------------------------------------------------
            PROBE:
                //std::cerr << "\tdefault step" << std::endl;
                SHFT(x.a,x.b,x.c,x.b + GOLD * (x.b - x.a) );
                SHFT(f.a,f.b,f.c,func(x.c));
                assert(f.b<=f.a);
                assert(x.is_ordered());
            }
            
            
        }
        
		
	} //math
	
}//yocto
