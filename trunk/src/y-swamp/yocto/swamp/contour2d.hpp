#ifndef YOCTO_SWAMP_CONTOUR2D_INCLUDED
#define YOCTO_SWAMP_CONTOUR2D_INCLUDED 1

#include "yocto/swamp/level-set.hpp"
#include "yocto/swamp/array2d.hpp"

namespace yocto 
{
    namespace swamp
    {
        template <typename T>
        struct contour2D
        {
            typedef typename vertex2D<T>::type                                        vertex;
            typedef functor<void,TL3(const vertex &,const vertex &,const level<T> &)> callback;
            
            static void compute(const array2D<T>   &d,
                                const array1D<T>   &x,
                                const array1D<T>   &y,
                                const layout2D     &sub,
                                const level_set<T> &levels,
                                callback            proc);
            
        };
    }
}


#endif
