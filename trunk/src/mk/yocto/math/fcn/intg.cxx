#include "yocto/math/ztype.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // range
        //
        ////////////////////////////////////////////////////////////////////////
        
        template <>
        integrator<real_t>:: range:: range( real_t A, real_t B, real_t EPS ) throw() :
        a(A),
        b(B),
        eps( max_of<real_t>(Fabs(EPS),numeric<real_t>::ftol) ),
        next(0)
        {
            
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // rpool
        //
        ////////////////////////////////////////////////////////////////////////
        
        
        template <>
        integrator<real_t>:: rpool:: rpool() throw() {}
        
        
        template <>
        void integrator<real_t>:: rpool:: cleanup() throw()
        {
            while( size )
            {
                object::release1<range>(query());
            }
        }
        
        template <>
        integrator<real_t>:: rpool:: ~rpool() throw()
        {
            cleanup();
        }
        
        template <>
        void   integrator<real_t>:: rpool:: pour_into( rpool &collector ) throw()
        {
            while( size ) collector.store( query() );
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // integrator
        //
        ////////////////////////////////////////////////////////////////////////
        
        namespace
        {
            
            template <size_t n>
            inline real_t dyadic_sum( numeric<real_t>::function &F, const real_t X, const real_t D )
            {
                assert(is_a_power_of_two(n));
                assert(n>1);
                static const size_t h=n>>1;
                return dyadic_sum<h>(F, X, D) + dyadic_sum<h>(F, X + (h*D), D);
            }
            
            template <>
            inline real_t dyadic_sum<1>( numeric<real_t>::function &F, const real_t X, const real_t D )
            {
                return F(X);
            }
            
            
            //! trapezoidal integration
            template <size_t n>
            static inline real_t  trpz(const real_t  s,
                                       const real_t  a,
                                       const real_t  w,
                                       numeric<real_t>::function &F)
            {
                static const size_t iter  = 1 << (n-2);
                const real_t        delta = w/iter;
                const real_t        start = a + REAL(0.5) * delta;
                const real_t        sum   = dyadic_sum<iter>(F, start,delta);
                return REAL(0.5)*(s+(w*sum)/iter);
            }
            
        }
        
        
#define Y_INTG_PROLOG(N)  \
st = trpz<N>(st,a,w,F); \
s = ( REAL(4.0) * st - old_st )/REAL(3.0)
        
#define Y_INTG_EPILOG()  \
old_st  = st;            \
old_s   = s
        
#define Y_INTG_CHECK() \
if( Fabs(s-old_s) <= Fabs( ftol * s ) )  return true
        
#define Y_INTG_FAST(N) \
Y_INTG_PROLOG(N); \
Y_INTG_EPILOG()
        
#define Y_INTG_TEST(N) \
Y_INTG_PROLOG(N); \
Y_INTG_CHECK();   \
Y_INTG_EPILOG()
        
        template <>
        bool integrator<real_t>:: quad(real_t &s,
                                       real_t  a,
                                       real_t  b,
                                       numeric<real_t>::function &F,
                                       real_t ftol)
        {
            const real_t w       = b-a;
            real_t       st      = REAL(0.5) * w * ( F(b)+F(a) );
            real_t       old_st  = st;
            s                    = st;
            real_t       old_s   = s;
            
            
            Y_INTG_FAST(2); // +1 : 3  evals
            Y_INTG_FAST(3); // +2 : 5  evals
            Y_INTG_FAST(4); // +4 : 9  evals
            Y_INTG_FAST(5); // +8 : 17 evals
            Y_INTG_FAST(6); // +16: 33 evals
            
            Y_INTG_TEST(7);  // +32:  65 evals
            Y_INTG_TEST(8);  // +64:  129 evals
            Y_INTG_TEST(9);  // +128: 257 evals
            Y_INTG_TEST(10); // +256: 513 evals
            Y_INTG_TEST(11); // +512: 1025 evals
            
            
            
            return false;
        }
        
        template <>
        integrator<real_t>:: integrator() throw() : pool()
        {
        }
        
        
        
        template <>
        integrator<real_t>:: ~integrator() throw()
        {
        }
        
        template <>
        integrator<real_t>:: range * integrator<real_t>::create(real_t a, real_t b, real_t ftol)
        {
            return new ( pool.size ? pool.query() : object::acquire1<range>() ) range(a,b,ftol);
        }
        
        
        
        template <>
        real_t integrator<real_t>:: operator()(real_t a,
                                               real_t b,
                                               numeric<real_t>::function &F,
                                               real_t ftol )
        {
            
            rpool  stk;
            real_t ans = 0;
            try
            {
                //______________________________________________________________
                //
                // initialize with full range
                //______________________________________________________________
                stk.store( create(a,b,ftol) );
                
                while( stk.size )
                {
                    real_t sum = 0;
                    //__________________________________________________________
                    //
                    // take full range
                    //__________________________________________________________
                    range *rng = stk.query();
                    const real_t a   = rng->a;
                    const real_t b   = rng->b;
                    const real_t eps = rng->eps;
                    pool.store(rng);
                    //std::cerr << "\tintg #" << stk.size << " [" << a << ":" << b << "]/eps=" << eps << std::endl;
                    if( quad(sum, a, b, F, eps) )
                    {
                        //______________________________________________________
                        //
                        // valid partial sum
                        //______________________________________________________
                        ans += sum;
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // subdivide
                        //______________________________________________________
                        const real_t c  = REAL(0.5) * (a+b);
                        const real_t e  = eps * REAL(0.5);
                        stk.store( create(a,c,e) );
                        stk.store( create(c,b,e) );
                    }
                }
                
                
                
            }
            catch(...)
            {
                stk.pour_into(pool);
                throw;
            }
            stk.pour_into(pool);
            
            return ans;
        }
        
    }
}
