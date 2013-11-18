/** \file  integrate.hpp
 \brief Integration of Functions.
 */

#ifndef YOCTO_INTEGRATE_INCLUDED
#define YOCTO_INTEGRATE_INCLUDED 1


#include "yocto/math/types.hpp"


namespace yocto {
    
	namespace math {
        
        template <typename T>
        struct intg
        {
            typedef functor<void,TL3(T,T&,T&)> boundaries;
        };
        
		//! recursive Simpson's integration.
        template <typename T>
        T integrate( T a, T b, typename  numeric<T>::function &F, T ftol );
        
        template <class T>
		T integrate(T                               xmin,
                    T                               xmax,
                    typename intg<T>::boundaries   &getYs,
                    typename numeric<T>::function2 &func2,
                    T                               ftol
                    );
        
    }
    
}

#endif
