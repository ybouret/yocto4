//! \file
/**
 Wrappers to use VisIt libsim.
 */
#ifndef YOCTO_SWAMP_VISIT_INCLUDED
#define YOCTO_SWAMP_VISIT_INCLUDED 1

#include <VisItDataInterface_V2.h>
#include "yocto/swamp/common.hpp"

namespace yocto 
{
    namespace swamp
    {
        
        struct _visit
        {
            
            //! mesh meta data
            template <typename T,typename LAYOUT>
            static inline
            visit_handle mesh_meta_data(const rmesh<T,LAYOUT> & mesh, 
                                        const string &          name,
                                        int                     num_domains )
            {
                assert(num_domains>0);
                visit_handle h = VISIT_INVALID_HANDLE;
                if( VisIt_MeshMetaData_alloc( &h ) == VISIT_OKAY )
                {
                    VisIt_MeshMetaData_setName( h, name.c_str() );
                    VisIt_MeshMetaData_setMeshType( h, VISIT_MESHTYPE_RECTILINEAR );
                    VisIt_MeshMetaData_setTopologicalDimension( h, LAYOUT::DIMENSIONS);
                    VisIt_MeshMetaData_setSpatialDimension(h, LAYOUT::DIMENSIONS);
                    VisIt_MeshMetaData_setNumDomains(h,num_domains);
                }
                return h;
            }
            
            //! mesh meta data
            template <typename T,typename LAYOUT>
            static inline
            visit_handle mesh_meta_data(const rmesh<T,LAYOUT> & mesh, 
                                        const char            * id,
                                        int                     num_domains )
            {
                const string name(id);
                return mesh_meta_data<T,LAYOUT>( mesh, name, num_domains );
            }
            
                        
            
        };
        
    }
}


#endif
