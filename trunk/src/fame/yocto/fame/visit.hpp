#ifndef YOCTO_FAME_VISIT_INCLUDED
#define YOCTO_FAME_VISIT_INCLUDED 1

#include "yocto/visit/visit.hpp"
#include "yocto/fame/mesh/rectilinear.hpp"

namespace yocto
{
    namespace fame
    {
        
        //! helper to send data to visit
        class visit
        {
        public:
            
            static inline void set_mesh_info( visit_handle &mmd, const MeshInfo &mesh )
            {
                VisIt_MeshMetaData_setName(mmd, mesh.name.c_str() );
                switch(mesh.dimensions)
                {
                    case 3:
                        VisIt_MeshMetaData_setZUnits(mmd, mesh.ZUnits().c_str() );
                        VisIt_MeshMetaData_setZLabel(mmd, mesh.ZLabel().c_str() );

                    case 2:
                        VisIt_MeshMetaData_setYUnits(mmd, mesh.YUnits().c_str() );
                        VisIt_MeshMetaData_setYLabel(mmd, mesh.YLabel().c_str() );
                        
                    case 1:
                        VisIt_MeshMetaData_setXUnits(mmd, mesh.XUnits().c_str() );
                        VisIt_MeshMetaData_setXLabel(mmd, mesh.XLabel().c_str() );
                        
                    default:
                        break;
                }
            }
            
            template <size_t DIM,typename T> static inline
            void add_mesh_meta_data( visit_handle &md, const RectilinearMesh<DIM,T> &mesh )
            {
                visit_handle mmd = VISIT_INVALID_HANDLE;
                if( VisIt_MeshMetaData_alloc(&mmd) == VISIT_OKAY)
                {
                    set_mesh_info(mmd,mesh);
                    VisIt_MeshMetaData_setMeshType(mmd, VISIT_MESHTYPE_RECTILINEAR);
                    VisIt_MeshMetaData_setTopologicalDimension(mmd,DIM);
                    VisIt_MeshMetaData_setSpatialDimension(mmd,DIM);
                    
                    VisIt_SimulationMetaData_addMesh(md, mmd);
                }
            }
            
            
            
        private:
            
        };
        
    }
}

#endif
