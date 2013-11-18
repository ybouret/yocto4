#include "yocto/math/fcn/integrate.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto {
	
	namespace math {
		
		static inline real_t pw_quad( numeric<real_t>::function &F, const real_t X, const real_t D, const uint32_t n )
		{
			assert(is_a_power_of_two(n));
			if(n>1) {
				const uint32_t h = n>>1; assert(n-h==h);
				return pw_quad(F,X,D,h) + pw_quad( F, X + (h*D), D, h );
			} else {
				return F(X);
			}
		}
		
		template <const uint8_t n >
		inline real_t _trapzd( const real_t s, const real_t a, const real_t w, numeric<real_t>::function &F ) {
#if ! defined( __clang__ )
			YOCTO_STATIC_CHECK(n>1,n_IS_TOO_SMALL);
			YOCTO_STATIC_CHECK(n<32,n_IS_TOO_BIG);
#endif
			static const uint32_t iter = static_cast<uint32_t>(1) << (n-2);
			const real_t del = w / iter;
			const real_t org = a + REAL(0.5) * del;
			const real_t sum = pw_quad( F, org, del, iter );
			return REAL(0.5) * ( s + ( w * sum ) / iter );
			
		}
		
		
#define SYQUO_QSIMP_(J)            \
st = _trapzd<J>(st,a,w,F);         \
s  = (REAL(4.0)*st-ost)/REAL(3.0); \
os = s;                            \
ost = st
		
#define SYQUO_QSIMP(J)                   \
st = _trapzd<J>(st,a,w,F);               \
s  = (REAL(4.0)*st-ost)/REAL(3.0);       \
if( Fabs(s-os) <= Fabs(ftol *os) )       \
{ return s; }                            \
os = s;                                  \
ost = st
		
		
		static
		real_t recursive_intg(  const real_t a, const real_t b, numeric<real_t>::function &F, const real_t ftol )
		{
			
			const real_t w = b-a;
			real_t ost=0,os=0;
			
			// J = 1: +2 -> 2 pts
			real_t st = REAL(0.5) * w * ( F(a) + F(b) );
			real_t s  = ( REAL(4.0) * st - ost ) / REAL(3.0);
			os = s;
			ost = st;
			
			SYQUO_QSIMP_(2);  // +   1 -> 3    pts
			SYQUO_QSIMP_(3);  // +   2 -> 5    pts
			SYQUO_QSIMP_(4);  // +   4 -> 9    pts
			SYQUO_QSIMP_(5);  // +   8 -> 17   pts
			
			SYQUO_QSIMP(6);   // +  16 -> 33   pts
			SYQUO_QSIMP(7);   // +  32 -> 65   pts
			SYQUO_QSIMP(8);   // +  64 -> 129  pts
			SYQUO_QSIMP(9);   // + 128 -> 257  pts
			SYQUO_QSIMP(10);  // + 256 -> 513  pts
			SYQUO_QSIMP(11);  // + 512 -> 1025 pts
			SYQUO_QSIMP(12);  // +1024 -> 2049 pts
			SYQUO_QSIMP(13);  // +2048 -> 4097 pts
			SYQUO_QSIMP(14);  // +4096 -> 8193 pts
			SYQUO_QSIMP(15);  // +8192 -> 16385 pts
			SYQUO_QSIMP(16);  // +16384 -> 32769 pts
			SYQUO_QSIMP(17);  // +32768 -> 65537 pts
			SYQUO_QSIMP(18);  // +65536 -> 131073 pts
			SYQUO_QSIMP(19);  // +131072-> 262145 pts
			SYQUO_QSIMP(20);  // +262144-> 524289 pts
			
			const real_t mid = (a+b) * REAL(0.5);
			return recursive_intg(a,mid,F,ftol) + recursive_intg(mid,b,F,ftol);
			
			
		}
		
		template <>
		real_t integrate<real_t>( real_t a, real_t b, numeric<real_t>::function &F,real_t ftol)
		{
			return recursive_intg(a,b,F,ftol);
		}
        
        namespace
        {
            class intg2proxy
            {
            public:
                inline intg2proxy(intg<real_t>::boundaries     &user_getYs,
                                  numeric<real_t>::function2   &user_func2,
                                  real_t                        user_ftol) :
                getYs(user_getYs),
                func2(user_func2),
                ftol( user_ftol )
                {
                    
                }
                
                intg<real_t>::boundaries    &getYs;
                numeric<real_t>::function2  &func2;
                const real_t                 ftol;
                
                inline real_t slice( real_t x )
                {
                    real_t ylo=0,yhi=0;
                    getYs(x,ylo,yhi);
                    numeric<real_t>::function F = bind_first(func2, x);
                    return integrate(ylo, yhi, F, ftol);
                }
                
                inline ~intg2proxy() throw() {}
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(intg2proxy);
            };
        }
        
        template <>
		real_t integrate(real_t                        xmin,
                         real_t                        xmax,
                         intg<real_t>::boundaries     &getYs,
                         numeric<real_t>::function2   &func2,
                         real_t                        ftol
                         )
        {
            intg2proxy                intg2( getYs, func2, ftol);
            numeric<real_t>::function intg1( &intg2, & intg2proxy::slice);
            
            return integrate<real_t>(xmin, xmax, intg1, ftol);
        }
        
		
	} // math
} //yocto

