#include "yocto/math/ztype.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/core/list.hpp"

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
        integrator<real_t>:: range:: range( real_t A, real_t B ) throw() :
        a(A),
        b(B),
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
            //! trapezoidal integration
            template <size_t n>
            real_t  trpz(const real_t  s,
                         const real_t  a,
                         const real_t  w,
                         numeric<real_t>::function &F)
            {
                static const size_t iter  = 1 << (n-2);
                const real_t        delta = w/iter;
                real_t              x     = a + REAL(0.5) * delta;
                real_t              sum   = REAL(0.0);
                for(size_t j=iter;j>0;--j,x+=delta)
                {
                    sum += F(x);
                }
                return REAL(0.5)*(s+sum*delta);
            }
            
        }
        
        
#define Y_INTG_PROLOG(N)  \
st = trpz<N>(st,a,w,F); \
s = ( REAL(4.0) * st - old_st )/REAL(3.0); std::cerr << "s" << N << "=" << s << std::endl
        
#define Y_INTG_EPILOG() \
old_st = st;            \
old_s  = s
        
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
            const real_t w      = b-a;
            real_t       st     = REAL(0.5) * w * ( F(b)+F(a) );
            real_t       old_st = st;
            s                   = st;
            real_t       old_s  = s;
            
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
        integrator<real_t>:: range * integrator<real_t>::query(real_t a, real_t b)
        {
            return new ( pool.size ? pool.query() : object::acquire1<range>() ) range(a,b);
        }
        
        
        template <>
        real_t integrator<real_t>:: operator()(real_t a,
                                               real_t b,
                                               numeric<real_t>::function &F,
                                               real_t ftol )
        {
            //__________________________________________________________________
            //
            // initialize with full range
            //__________________________________________________________________
            rpool stk;
            try
            {
                
            }
            catch(...)
            {
                stk.pour_into(pool);
                throw;
            }
            stk.pour_into(pool);
            
            return 0;
        }
        
    }
}
