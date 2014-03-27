#ifndef YOCTO_FOVEA_ARRAYS_INCLUDED
#define YOCTO_FOVEA_ARRAYS_INCLUDED 1

#include "yocto/fovea/array3d.hpp"

namespace yocto
{
    namespace fovea
    {
        
        template <size_t DIM, typename T>
        struct array_of;
        
        template <typename T>
        struct array_of<1,T>
        {
            typedef array1D<T>                 type;
            typedef typename vertex1D<T>::type vertex;
        };
        
        template <typename T>
        struct array_of<2,T>
        {
            typedef array2D<T>                 type;
            typedef typename vertex1D<T>::type vertex;
        };
        
        template <typename T>
        struct array_of<3,T>
        {
            typedef array3D<T>                 type;
            typedef typename vertex3D<T>::type vertex;
        };

        
    }
    
}

#endif
