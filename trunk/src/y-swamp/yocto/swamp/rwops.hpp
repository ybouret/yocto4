#ifndef YOCTO_SWAMP_RWOPS_INCLUDED
#define YOCTO_SWAMP_RWOPS_INCLUDED 1

#include "yocto/swamp/triangle.hpp"
#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/swamp/rmesh.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        template <typename T>
        struct rwops
        {
            
            static
            void save_vtk(const string                       &filename,
                          const string                       &title,
                          const workspace<layout2D,T,rmesh>  &wksp,
                          const array<string>                &id,
                          const layout2D                     &sub );
            
            static
            void save_vtk(const string                       &filename,
                          const string                       &title,
                          const workspace<layout3D,T,rmesh>  &wksp,
                          const array<string>                &id,
                          const layout3D                     &sub );
            
            static  
            void save_vtk( const string &filename, const string &title, const array< triangle3D<T> > &triangles);
            
            
            
        };
        
    }
    
}


#endif
