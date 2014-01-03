#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/implicit/rosenbrock.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace ode
        {

            template <>
            rosenbrock<real_t>:: ~rosenbrock() throw() {}
            
            template <>
            rosenbrock<real_t>:: rosenbrock() : 
            implicit_step<real_t>( 10 ),
            g1(    next_array() ),
            g2(    next_array() ),
            g3(    next_array() ),
            g4(    next_array() ),
            dfdx(  next_array() ),
            err(   next_array() ),
            ysav(  next_array() ),
            dysav( next_array() ),
            dfdy(),
            a(),
            LU()
            {
            }
            
            template <>
            void rosenbrock<real_t>:: prepare( size_t nv )
            {
                assert(nv>0);
                implicit_step<real_t>::prepare(nv);
                dfdy.make(nv,nv);
                a.make(nv,nv);
                LU.ensure(nv);
            }
            
        }


    }

}
