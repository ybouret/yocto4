#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/rosenbrock.hpp"

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
            stiff_step<real_t>( 10 ),
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
            lss()
            {
            }
            
            template <>
            void rosenbrock<real_t>:: start( size_t nv )
            {
                assert(nv>0);
                prepare(nv);
                dfdy.make(nv,nv);
                a.make(nv,nv);
                lss.ensure(nv);
            }
            
        }


    }

}
