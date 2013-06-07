/**
 \file
 \brief Wrappers to use VisIt libsim.
 */
#ifndef YOCTO_SWAMP_VISIT_INCLUDED
#define YOCTO_SWAMP_VISIT_INCLUDED 1

#include <VisItDataInterface_V2.h>
#include "yocto/spade/rmesh.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/math/v3d.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace spade
    {
        
        //! Wrapper for common VisIt operations
        class VisItIO
        {
        public:
            class param
            {
            public:
                const type_spec         spec;       //!< the type spec
                const int               vartype;    //!< VISIT_VARTYPE_(SCALAR|VECTOR)
                const int               components; //!< nComponents
                inline const type_spec &key() const throw() { return spec; }
                
                inline param( const std::type_info &id, int vt, int nc) :
                spec(id),
                vartype(vt),
                components(nc)
                {
                }
                
                inline param( const param &other ) :
                spec( other.spec ),
                vartype( other.vartype ),
                components( other.components )
                {
                }
                
                inline ~param() throw() {}
                
            private:
                YOCTO_DISABLE_ASSIGN(param);
            };
            set<type_spec,param> params;
            
            
            explicit VisItIO() : params(8,as_capacity)
            {
                { const param p( typeid(float),             VISIT_VARTYPE_SCALAR, 1);  record(p); }
                { const param p( typeid(double),            VISIT_VARTYPE_SCALAR, 1);  record(p); }
                { const param p( typeid(math::v2d<float>),  VISIT_VARTYPE_VECTOR, 2);  record(p); }
                { const param p( typeid(math::v2d<double>), VISIT_VARTYPE_VECTOR, 2);  record(p); }
                { const param p( typeid(math::v3d<float>),  VISIT_VARTYPE_VECTOR, 3);  record(p); }
                { const param p( typeid(math::v3d<double>), VISIT_VARTYPE_VECTOR, 3);  record(p); }
                
            }
            virtual ~VisItIO() throw() {}
            
            //! mesh meta data
            /**
             declare a rectilinear mesh for VisIt
             \param mesh the rectilinear mesh
             \param name the name as seen by VisIt
             \param num_domains should be par_size for standard simuliation
             \param labels labels for axes, should have #DIMENSIONS
             */
            template <typename T,typename LAYOUT>
            inline
            visit_handle mesh_meta_data(const rmesh<LAYOUT,T> & mesh,
                                        const string &          name,
                                        int                     num_domains,
                                        char                  **labels = 0,
                                        char                  **units  = 0
                                        ) const
            {
                assert(num_domains>0);
                visit_handle h = VISIT_INVALID_HANDLE;
                if( VisIt_MeshMetaData_alloc( &h ) == VISIT_OKAY )
                {
                    VisIt_MeshMetaData_setName( h, name.c_str() );
                    VisIt_MeshMetaData_setMeshType( h, VISIT_MESHTYPE_RECTILINEAR );
                    VisIt_MeshMetaData_setTopologicalDimension( h, LAYOUT::DIMENSIONS);
                    VisIt_MeshMetaData_setSpatialDimension(h, LAYOUT::DIMENSIONS);
                    
                    if(labels)
                    {
                        VisIt_MeshMetaData_setXLabel(h, labels[0]);
                        if( LAYOUT::DIMENSIONS > 1 )
                            VisIt_MeshMetaData_setYLabel(h, labels[1]);
                        if( LAYOUT::DIMENSIONS > 2 )
                            VisIt_MeshMetaData_setZLabel(h, labels[2]);
                    }
                    
                    if(units)
                    {
                        VisIt_MeshMetaData_setXUnits(h, units[0]);
                        if( LAYOUT::DIMENSIONS > 1 )
                            VisIt_MeshMetaData_setYUnits(h, units[1]);
                        if( LAYOUT::DIMENSIONS > 2 )
                            VisIt_MeshMetaData_setZUnits(h, units[2]);
                        
                    }
                    
                    VisIt_MeshMetaData_setNumDomains(h,num_domains);
                }
                else
                    throw exception("VisIt I/O::mesh_meta_data error");
                return h;
            }
            
            //! mesh meta data wrapper
            template <typename T,typename LAYOUT>
            inline
            visit_handle mesh_meta_data(const rmesh<LAYOUT,T> & mesh,
                                        const char            * id,
                                        int                     num_domains,
                                        char                  **labels = 0,
                                        char                  **units  = 0) const
            {
                const string name(id);
                return mesh_meta_data<T,LAYOUT>( mesh, name, num_domains,labels,units );
            }
            
            //! variable meta data
            /**
             create a variable meta data to be attached to a mesh.
             */
            template <typename T>
            inline visit_handle variable_meta_data(const string &name,
                                                   const string &mesh_id,
                                                   const char   *units = 0
                                                   ) const
            {
                //! query the variable type id
                const type_spec spec( typeid(T) );
                const param    *p = params.search( spec );
                if( !p )
                    throw exception("no VisIt I/O for <'%s'>", spec.name());
                
                //! create the meta data
                visit_handle vmd = VISIT_INVALID_HANDLE;
                if(VisIt_VariableMetaData_alloc(&vmd) == VISIT_OKAY)
                {
                    VisIt_VariableMetaData_setName(vmd, name.c_str());
                    VisIt_VariableMetaData_setMeshName(vmd, mesh_id.c_str());
                    VisIt_VariableMetaData_setType(vmd, p->vartype);
                    VisIt_VariableMetaData_setCentering(vmd, VISIT_VARCENTERING_NODE);
                    if(units)
                        VisIt_VariableMetaData_setUnits(vmd, units );
                }
                else
                    throw exception("VisIt I/O::variable_data error");
                return vmd;
            }
            
            //! variable meta data wrapper
            template <typename T>
            inline visit_handle variable_meta_data(const char *nid,
                                                   const char *mid,
                                                   const char *units = 0) const
            {
                const string name(nid), mesh_id(mid);
                return variable_meta_data<T>(name,mesh_id,units);
            }
            
        private:
            inline void record( const param &p ) { (void) params.insert(p); }
            YOCTO_DISABLE_COPY_AND_ASSIGN(VisItIO);
            
        };
        
    }
}


#endif
