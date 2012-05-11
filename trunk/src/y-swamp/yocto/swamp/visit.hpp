//! \file
/**
 Wrappers to use VisIt libsim.
 */
#ifndef YOCTO_SWAMP_VISIT_INCLUDED
#define YOCTO_SWAMP_VISIT_INCLUDED 1

#include <VisItDataInterface_V2.h>
#include "yocto/swamp/common.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace swamp
    {
        
        class _visit
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
            
            
            explicit _visit() : params(8,as_capacity) 
            {
                { const param p( typeid(float),             VISIT_VARTYPE_SCALAR, 1 ); record(p); }
                { const param p( typeid(double),            VISIT_VARTYPE_SCALAR, 1 ); record(p); }
                { const param p( typeid(geom::v2d<float>),  VISIT_VARTYPE_VECTOR, 2);  record(p); }
                { const param p( typeid(geom::v2d<double>), VISIT_VARTYPE_VECTOR, 2);  record(p); }
                { const param p( typeid(geom::v3d<float>),  VISIT_VARTYPE_VECTOR, 3);  record(p); }
                { const param p( typeid(geom::v3d<double>), VISIT_VARTYPE_VECTOR, 3);  record(p); }
                
            }
            virtual ~ _visit() throw() {}
            
            //! mesh meta data
            template <typename T,typename LAYOUT>
            inline
            visit_handle mesh_meta_data(const rmesh<T,LAYOUT> & mesh, 
                                        const string &          name,
                                        int                     num_domains ) const
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
                else 
                    throw exception("_visit::mesh_meta_data error");
                return h;
            }
            
            //! mesh meta data
            template <typename T,typename LAYOUT>
            inline
            visit_handle mesh_meta_data(const rmesh<T,LAYOUT> & mesh, 
                                        const char            * id,
                                        int                     num_domains ) const
            {
                const string name(id);
                return mesh_meta_data<T,LAYOUT>( mesh, name, num_domains );
            }
            
            //! variable meta data
            template <typename T>
            inline visit_handle variable_meta_data( const string &name, const string &mesh_id ) const
            {
                const type_spec spec( typeid(T) );
                const param    *p = params.search( spec );
                if( !p )
                    throw exception("no _visit('%s')", spec.name());
                
                visit_handle vmd = VISIT_INVALID_HANDLE;
                if(VisIt_VariableMetaData_alloc(&vmd) == VISIT_OKAY)
                {
                    VisIt_VariableMetaData_setName(vmd, name.c_str());
                    VisIt_VariableMetaData_setMeshName(vmd, mesh_id.c_str());
                    VisIt_VariableMetaData_setType(vmd, p->vartype);
                    VisIt_VariableMetaData_setCentering(vmd, VISIT_VARCENTERING_NODE);
                }
                else 
                    throw exception("_visit::variable_data error");
                return vmd;
            }
            
            template <typename T>
            inline visit_handle variable_meta_data( const char *nid, const char *mid ) const
            {
                const string name(nid), mesh_id(mid);
                return variable_meta_data<T>(name,mesh_id);
            }
            
        private:
            inline void record( const param &p ) { (void) params.insert(p); }
            YOCTO_DISABLE_COPY_AND_ASSIGN(_visit);
            
        };
        
    }
}


#endif
