#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sort/network.hpp"

#include "yocto/ios/ocstream.hpp"

namespace yocto {

    namespace math {

#define XTOL numeric<real_t>::sqrt_ftol

        static inline
        real_t __compute_ftol(real_t ftol) throw()
        {
            return max_of(Fabs(ftol),XTOL);
        }



        namespace kernel
        {
            template <>
            void minimize(numeric<real_t>::function &func,
                          triplet<real_t>           &x,
                          triplet<real_t>           &f)
            {
                static const real_t C = REAL(0.381966011250105);
                assert(x.a<=x.b);
                assert(x.b<=x.c);
                assert(f.b<=f.a);
                assert(f.b<=f.c);
                const real_t ab = max_of<real_t>(x.b - x.a,0);
                const real_t bc = max_of<real_t>(x.c - x.b,0);
                if(bc>ab)
                {
                    //__________________________________________________________
                    //
                    // probe at right
                    //__________________________________________________________
                    const real_t xu = clamp(x.b,x.b+C*bc,x.c);
                    const real_t fu = func(xu);
                    if(fu>=f.b)
                    {
                        // move c->u
                        x.c = xu;
                        f.c = fu;
                    }
                    else
                    {
                        // move a->b, b->u
                        x.a = x.b;
                        f.a = f.b;
                        x.b = xu;
                        f.b = fu;
                    }
                }
                else
                {
                    //__________________________________________________________
                    //
                    // probe at left
                    //__________________________________________________________
                    const real_t xu = clamp(x.a,x.b-C*ab,x.b);
                    const real_t fu = func(xu);
                    if(fu>=f.b)
                    {
                        // move a->u
                        x.a = xu;
                        f.a = fu;
                    }
                    else
                    {
                        // move c->b, b->u
                        x.c = x.b;
                        f.c = f.b;
                        x.b = xu;
                        f.b = fu;
                    }
                }
                x.co_sort(f);
            }
        }

        template <>
        void minimize<real_t>(numeric<real_t>::function &func,
                              triplet<real_t>           &x,
                              triplet<real_t>           &f,
                              real_t                     ftol )
        {
            assert(x.is_ordered());
            x.co_sort(f);
            assert(x.a<=x.b);
            assert(x.b<=x.c);
            assert(f.b<=f.a);
            assert(f.b<=f.c);
            ftol =  __compute_ftol(ftol);

            while( max_of<real_t>(x.c-x.a,0) > max_of(ftol*Fabs(x.b),XTOL) )
            {
                kernel::minimize<real_t>(func,x,f);
            }

        }

        
    } // math
    
} //yocto
