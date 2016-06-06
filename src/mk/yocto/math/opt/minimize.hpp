/** \file minimize.hpp
 */

#ifndef YOCTO_OPT_MINIMIZE_INCLUDED
#define YOCTO_OPT_MINIMIZE_INCLUDED 1


#include "yocto/math/triplet.hpp"

namespace yocto {

    namespace math {

        namespace kernel
        {
            template <typename T>
            void minimize(typename numeric<T>::function &func,
                          triplet<T> &x,
                          triplet<T> &f);

        }

        //! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
        /**
         ftol will be internally checked
         */
        template <typename T>
        void minimize(typename numeric<T>::function &func,
                      triplet<T> &x,
                      triplet<T> &f,
                      T ftol);

        //! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
        /**
         based on xtol or re-increasing space
         */
        template <typename T>
        void optimize(typename numeric<T>::function &func,
                      triplet<T> &x,
                      triplet<T> &f,
                      T          xtol);

    }
    
}

#endif
