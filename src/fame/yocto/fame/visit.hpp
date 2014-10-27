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
            
            //__________________________________________________________________
            //
            // declaring mesh meta data
            //__________________________________________________________________
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
            
            //__________________________________________________________________
            //
            // sending mesh data
            //__________________________________________________________________
            template <size_t DIM,typename T> static inline
            visit_handle get_mesh( const RectilinearMesh<DIM,T> &mesh )
            {
                visit_handle h = VISIT_INVALID_HANDLE;
                
                if( VisIt_RectilinearMesh_alloc(&h) == VISIT_OKAY )
                {
                    visit_handle hc[DIM];
                    for(size_t i=0;i<DIM;++i)
                    {
                        visit_handle &hh = hc[i];
                        VisIt_VariableData_alloc( &hh );
                        const typename RectilinearMesh<DIM,T>::Axis &a = mesh.get_axis(i);
                        hook_data(hh,a.items,a.entry);
                    }
                    set_coordinates(h, hc, int2type<DIM>() );
                }
                return h;
            }
            
            //__________________________________________________________________
            //
            // declaring data
            //__________________________________________________________________
            template <typename ARRAY> static inline
            void add_variable_meta_data(visit_handle &md,
                                        const ARRAY   &a,
                                        const string &mesh
                                        )
            {
                visit_handle vmd = VISIT_INVALID_HANDLE;
                if( VisIt_VariableMetaData_alloc(&vmd) == VISIT_OKAY )
                {
                    VisIt_VariableMetaData_setName(vmd, a.name.c_str() );
                    VisIt_VariableMetaData_setMeshName(vmd, mesh.c_str() );
                    VisIt_VariableMetaData_setCentering(vmd, VISIT_VARCENTERING_NODE);
                    set_variable_meta_data(vmd, type2type< typename ARRAY::type >() );
                    VisIt_SimulationMetaData_addVariable(md, vmd);
                }
            }
            
            
        private:
            //__________________________________________________________________
            //
            //
            // MESHES
            //
            //__________________________________________________________________
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
            
            
            static inline void hook_data(visit_handle &h, size_t num, float *reg)
            {
                VisIt_VariableData_setDataF(h, VISIT_OWNER_SIM, 1, num, reg);
            }
            
            static inline void hook_data(visit_handle &h, size_t num, double *reg)
            {
                VisIt_VariableData_setDataD(h, VISIT_OWNER_SIM, 1, num, reg);
            }
            
            static inline void set_coordinates(visit_handle &h, visit_handle *hc, int2type<2>)
            {
                VisIt_RectilinearMesh_setCoordsXY(h,hc[0], hc[1]);
            }
            
            static inline void set_coordinates(visit_handle &h, visit_handle *hc, int2type<3>)
            {
                VisIt_RectilinearMesh_setCoordsXYZ(h,hc[0], hc[1], hc[2]);
            }
            
            //__________________________________________________________________
            //
            //
            // data
            //
            //__________________________________________________________________
            static inline
            void set_variable_meta_data( visit_handle &vmd, type2type<float> )
            {
                VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_SCALAR );
            }
            
            static inline
            void set_variable_meta_data( visit_handle &vmd, type2type<double> )
            {
                VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_SCALAR );
            }
           
            static inline
            void set_variable_meta_data( visit_handle &vmd, type2type< math::v2d<float> > )
            {
                VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_VECTOR );
                VisIt_VariableMetaData_setNumComponents(vmd, 2);
            }
            
            static inline
            void set_variable_meta_data( visit_handle &vmd, type2type< math::v2d<double> > )
            {
                VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_VECTOR );
                VisIt_VariableMetaData_setNumComponents(vmd, 2);
            }

            static inline
            void set_variable_meta_data( visit_handle &vmd, type2type< math::v3d<float> > )
            {
                VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_VECTOR );
                VisIt_VariableMetaData_setNumComponents(vmd, 3);
            }
            
            static inline
            void set_variable_meta_data( visit_handle &vmd, type2type< math::v3d<double> > )
            {
                VisIt_VariableMetaData_setType(vmd, VISIT_VARTYPE_VECTOR );
                VisIt_VariableMetaData_setNumComponents(vmd, 3);
            }
            
            
        };
        
    }
}

#endif
