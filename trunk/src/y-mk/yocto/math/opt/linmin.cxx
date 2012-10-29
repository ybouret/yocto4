#include "yocto/math/ztype.hpp"
#include "yocto/math/opt/linmin.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

namespace yocto
{
    
    namespace math
    {
        
        namespace
        {
            // linmin wrapper
            class lm_wrapper
            {
            public:
                inline lm_wrapper(numeric<real_t>::scalar_field &func,
                                  const array<real_t>           &p,
                                  const array<real_t>           &g) :
                _func( func ),
                _p( p ),
                _g( g ),
                xx( p.size(), 0 )
                {
                    
                }
                
                numeric<real_t>::scalar_field & _func;
                const array<real_t>           & _p;
                const array<real_t>           & _g;
                vector<real_t>                  xx;
                
                inline ~lm_wrapper() throw() {}
                
                inline real_t compute( real_t x )
                {
                    for( size_t i=xx.size();i>0;--i)
                    {
                        xx[i] = _p[i] + x * _g[i];
                    }
                    return _func(xx);
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(lm_wrapper);
            };
        }
        
        template <>
        bool linmin<real_t>(numeric<real_t>::scalar_field &func,
                            const array<real_t>           &p,
                            const array<real_t>           &g,
                            triplet<real_t>               &x,
                            triplet<real_t>               &f,
                            const real_t                   ftol
                            )
        {
            lm_wrapper                w(func,p,g);
            numeric<real_t>::function F(&w,&lm_wrapper::compute);
            bracket<real_t>::expand(F, x, f);
            minimize<real_t>(F, x, f, ftol);
            return true;
        }
    }
    
}
