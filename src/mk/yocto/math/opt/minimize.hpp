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

        
        template <typename T>
        struct optimize1D
        {
            //! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
            /**
             based on xtol or re-increasing space
             */
            static void run(typename numeric<T>::function &func,
                            triplet<T> &x,
                            triplet<T> &f,
                            T          xtol);
            
            //! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
            /**
             - return true is isOK(x,f) returns true
             - return false if xtol is reached or re-increasing space
             */
            typedef functor<bool,TL2(const triplet<T>&,const triplet<T>&)> event;
            static bool run_until(event                         &isOK,
                                  typename numeric<T>::function &func,
                                  triplet<T>                    &x,
                                  triplet<T>                    &f,
                                  T                              xtol);

        };

    }
    
}

#endif
