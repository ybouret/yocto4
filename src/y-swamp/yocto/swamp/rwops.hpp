#ifndef YOCTO_SWAMP_RWOPS_INCLUDED
#define YOCTO_SWAMP_RWOPS_INCLUDED 1

#include "yocto/swamp/triangle.hpp"
#include "yocto/swamp/workspace.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        template <typename T>
        struct rwops
        {
            
            static  
            void save_vtk( const string &filename, const string &title, const array< triangle3D<T> > &triangles);
            
            
        };
        
    }
    
}


#endif
