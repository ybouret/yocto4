#ifndef YOCTO_FAME_VISIT_INCLUDED
#define YOCTO_FAME_VISIT_INCLUDED 1

#include "yocto/visit/visit.hpp"
#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/fame/curv.hpp"


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
            template <typename Layout,typename T> static inline
            void add_mesh_meta_data( visit_handle                   &md,
                                    const RectilinearMesh<Layout,T> &mesh,
                                    const int num_domains
                                    )
            {
                assert(num_domains>0);
                visit_handle mmd = VISIT_INVALID_HANDLE;
                const size_t dim = Layout::DIMENSIONS;
                if( VisIt_MeshMetaData_alloc(&mmd) == VISIT_OKAY)
                {
                    set_mesh_info(mmd,mesh);
                    VisIt_MeshMetaData_setMeshType(mmd, VISIT_MESHTYPE_RECTILINEAR);
                    VisIt_MeshMetaData_setTopologicalDimension(mmd,dim);
                    VisIt_MeshMetaData_setSpatialDimension(mmd,dim);
                    VisIt_MeshMetaData_setNumDomains(mmd,num_domains);
                    
                    VisIt_SimulationMetaData_addMesh(md, mmd);
                }
            }
            
            
            //__________________________________________________________________
            //
            // sending mesh data
            //__________________________________________________________________
            template <typename Layout,typename T> static inline
            visit_handle get_mesh(const RectilinearMesh<Layout,T> &mesh,
                                  const int )
            {
                visit_handle h = VISIT_INVALID_HANDLE;
                if( VisIt_RectilinearMesh_alloc(&h) == VISIT_OKAY )
                {
                    // raw parameters
                    visit_handle hc[Layout::DIMENSIONS];
                    for(size_t i=0;i<Layout::DIMENSIONS;++i)
                    {
                        visit_handle &hh = hc[i];
                        VisIt_VariableData_alloc( &hh );
                        const typename RectilinearMesh<Layout,T>::Axis &a = mesh.get_axis(i);
                        hook_data(hh,a.items,a.entry);
                    }
                    set_coordinates(h, hc, int2type<Layout::DIMENSIONS>() );
                    
                    // ghosts zone
                    int minRealIndex[Layout::DIMENSIONS];
                    int maxRealIndex[Layout::DIMENSIONS];
                    
                    for(size_t dim=0;dim<Layout::DIMENSIONS;++dim)
                    {
                        minRealIndex[dim] = 0;
                        maxRealIndex[dim] = __coord(mesh.outline.width,dim) - 1;
                        
                        if(__coord(mesh.ranks,dim)>0)
                        {
                            const quad_links &links = mesh.links(dim);
                            for(size_t i=0;i<links.count;++i)
                            {
                                if(links[i].pos==quad_link::is_prev)
                                {
                                    minRealIndex[dim] = 1; // TODO: check delta outline/layout
                                }
                            }
                        }
                    }
                    
                    
                    VisIt_RectilinearMesh_setRealIndices(h, minRealIndex, maxRealIndex);
                    
                }
                return h;
            }
            
            
            //__________________________________________________________________
            //
            // declaring variable meta data
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
            
            
            //__________________________________________________________________
            //
            // sending variable data
            //__________________________________________________________________
            template <typename ARRAY> static inline
            visit_handle get_variable_data( const ARRAY &a, int )
            {
                visit_handle h = VISIT_INVALID_HANDLE;
                if( VisIt_VariableData_alloc(&h) == VISIT_OKAY )
                {
                    set_variable_data(h,
                                      type2type< typename ARRAY::type>(),
                                      a.entry,
                                      a.items);
                }
                return h;
            }
            
            //__________________________________________________________________
            //
            // declaring curve meta data
            //__________________________________________________________________
            static inline
            void add_curv_meta_data(visit_handle   &md,
                                    const CurvInfo &curv
                                    )
            {
                visit_handle h = VISIT_INVALID_HANDLE;
                if( VisIt_CurveMetaData_alloc(&h) == VISIT_OKAY )
                {
                    VisIt_CurveMetaData_setName(h,   curv.name.c_str()   );
                    VisIt_CurveMetaData_setXLabel(h, curv.XLabel.c_str() );
                    VisIt_CurveMetaData_setYLabel(h, curv.YLabel.c_str() );
                    
                    VisIt_SimulationMetaData_addCurve(md, h);
                }
            }
            
            template <typename T> static inline
            visit_handle get_curv( const array1D<T> &cx, const array1D<T> &cy )
            {
                visit_handle h = VISIT_INVALID_HANDLE;
                
                if( (cx.items == cy.items) && VisIt_CurveData_alloc(&h) == VISIT_OKAY )
                {
                    visit_handle hxc, hyc;
                    VisIt_VariableData_alloc(&hxc);
                    VisIt_VariableData_alloc(&hyc);
                    hook_data(hxc, cx.items, cx.entry);
                    hook_data(hyc, cy.items, cy.entry);
                    VisIt_CurveData_setCoordsXY(h, hxc, hyc);
                    return h;
                }
                return h;
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
            void set_variable_meta_data( visit_handle &vmd, type2type<int> )
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
            
            static inline
            void set_variable_data(visit_handle &h,
                                   type2type<float>,
                                   const void  *entry,
                                   const size_t items)
            {
                VisIt_VariableData_setDataF(h,VISIT_OWNER_SIM, 1, items, (float *)entry);
            }
            
            static inline
            void set_variable_data(visit_handle &h,
                                   type2type<double>,
                                   const void  *entry,
                                   const size_t items)
            {
                VisIt_VariableData_setDataD(h,VISIT_OWNER_SIM, 1, items, (double *)entry);
            }
            
            static inline
            void set_variable_data(visit_handle &h,
                                   type2type<int>,
                                   const void  *entry,
                                   const size_t items)
            {
                VisIt_VariableData_setDataI(h,VISIT_OWNER_SIM, 1, items, (int *)entry);
            }
            
            static inline
            void set_variable_data(visit_handle &h,
                                   type2type< math::v2d<float> >,
                                   const void  *entry,
                                   const size_t items)
            {
                VisIt_VariableData_setDataF(h,VISIT_OWNER_SIM, 2, items, (float *)entry);
            }
            
            static inline
            void set_variable_data(visit_handle &h,
                                   type2type< math::v2d<double> >,
                                   const void  *entry,
                                   const size_t items)
            {
                VisIt_VariableData_setDataD(h,VISIT_OWNER_SIM, 2, items, (double *)entry);
            }
            
            static inline
            void set_variable_data(visit_handle &h,
                                   type2type< math::v3d<float> >,
                                   const void  *entry,
                                   const size_t items)
            {
                VisIt_VariableData_setDataF(h,VISIT_OWNER_SIM, 3, items, (float *)entry);
            }
            
            static inline
            void set_variable_data(visit_handle &h,
                                   type2type< math::v3d<double> >,
                                   const void  *entry,
                                   const size_t items)
            {
                VisIt_VariableData_setDataD(h,VISIT_OWNER_SIM, 3, items, (double *)entry);
            }
            
            
            
            
        };
        
    }
}

#endif
