#ifndef YOCTO_SWAMP_REGIONS_INCLUDED
#define YOCTO_SWAMP_REGIONS_INCLUDED 1

#include "yocto/swamp/in1d.hpp"
#include "yocto/swamp/in2d.hpp"
#include "yocto/swamp/in3d.hpp"

namespace yocto 
{
    
    namespace swamp 
    {
        
        template <typename U, size_t DIM> struct __region;
        
        template <typename U>
        struct __region<U,1> { 
            typedef typename region1D<U>::type type; 
            typedef typename type::vertex      vertex;
        };
        
        template <typename U>
        struct __region<U,2> { 
            typedef typename region2D<U>::type type;
            typedef typename type::vertex      vertex;
        };
        
        
        template <typename U>
        struct __region<U,3> { 
            typedef typename region3D<U>::type type;
            typedef typename type::vertex      vertex;
        };
        
        
    }
    
}


#endif
