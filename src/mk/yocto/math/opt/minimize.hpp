/** \file minimize.hpp
 */

#ifndef YOCTO_OPT_MINIMIZE_INCLUDED
#define YOCTO_OPT_MINIMIZE_INCLUDED 1


#include "yocto/math/triplet.hpp"

namespace yocto {
    
	namespace math {
        
        
        //! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
        /**
         ftol wil be internally >= sqrt(epsilon)
         */
        template <typename T>
        void minimize( typename numeric<T>::function &func, triplet<T> &x, triplet<T> &f, T xtol);
        
        
        //! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
        template <typename T>
        void minimize2( typename numeric<T>::function &func, triplet<T> &x, triplet<T> &f, T xtol);

        
#if 0
        //! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
        template <typename T>
        void minimize3( typename numeric<T>::function &func, triplet<T> &x, triplet<T> &f, T xtol);
#endif
        
        
	}
    
}

#endif
