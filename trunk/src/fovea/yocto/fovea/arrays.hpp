#ifndef YOCTO_FOVEA_ARRAYS_INCLUDED
#define YOCTO_FOVEA_ARRAYS_INCLUDED 1

#include "yocto/fovea/array3d.hpp"

namespace yocto
{
    namespace fovea
    {
        
        template <size_t DIM, typename T>
        struct types_for;
        
        template <typename T>
        struct types_for<1,T>
        {
            typedef array1D<T>                 array_type;
            typedef typename vertex1D<T>::type vertex_type;
        };
        
        template <typename T>
        struct types_for<2,T>
        {
            typedef array2D<T>                 array_type;
            typedef typename vertex1D<T>::type vertex_type;
        };
        
        template <typename T>
        struct types_for<3,T>
        {
            typedef array3D<T>                 array_type;
            typedef typename vertex3D<T>::type vertex_type;
        };

        
    }
    
}

#endif
