#ifndef YOCTO_MATH_OPT_LINMIN_INCLUDED
#define YOCTO_MATH_OPT_LINMIN_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/math/triplet.hpp"

namespace yocto
{
    namespace math
    {
        
        //! linear minimisation
        /**
         minimize func(p+x*g), starting from
         x.a and x.b with f.a and f.b already computed
         */
        template <typename T>
        bool linmin(typename numeric<T>::scalar_field &func,
                    const array<T>                    &p,
                    const array<T>                    &g,
                    triplet<T>                        &x,
                    triplet<T>                        &f,
                    const T                            ftol
                    );
        
    }
}

#endif
