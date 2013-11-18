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
            typedef void (*set_data_proc)( visit_handle &, int, int, const void *);
            class param
            {
            public:
                const type_spec         spec;       //!< the type spec
                const int               vartype;    //!< VISIT_VARTYPE_(SCALAR|VECTOR)
                const int               components; //!< nComponents
                set_data_proc           set_data;   //!< for variable_data function
                inline const type_spec &key() const throw() { return spec; }
                
                inline param( const std::type_info &id, int vt, int nc, set_data_proc sd) :
                spec(id),
                vartype(vt),
                components(nc),
                set_data(sd)
                {
                    assert(set_data!=0);
                }
                
                inline param( const param &other ) :
                spec(       other.spec       ),
                vartype(    other.vartype    ),
                components( other.components ),
                set_data(   other.set_data   )
                {
                }
                
                inline ~param() throw() {}
                
            private:
                YOCTO_DISABLE_ASSIGN(param);
            };
            set<type_spec,param> params;
            
            
            explicit VisItIO() : params(8,as_capacity)
            {
                { const param p( typeid(float),             VISIT_VARTYPE_SCALAR, 1, set_data_f);  record(p); }
                { const param p( typeid(double),            VISIT_VARTYPE_SCALAR, 1, set_data_d);  record(p); }
                { const param p( typeid(int),               VISIT_VARTYPE_SCALAR, 1, set_data_i);  record(p); }
                { const param p( typeid(math::v2d<float>),  VISIT_VARTYPE_VECTOR, 2, set_data_f);  record(p); }
                { const param p( typeid(math::v2d<double>), VISIT_VARTYPE_VECTOR, 2, set_data_d);  record(p); }
                { const param p( typeid(math::v3d<float>),  VISIT_VARTYPE_VECTOR, 3, set_data_f);  record(p); }
                { const param p( typeid(math::v3d<double>), VISIT_VARTYPE_VECTOR, 3, set_data_d);  record(p); }
                
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
                    throw exception("VisIt I/O::variable_meta_data error");
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
            
            template <typename ARRAY>
            inline visit_handle variable_data( const ARRAY &A ) const
            {
                assert(A.items>0);
                assert(A.entry!=NULL);
                
                //--------------------------------------------------------------
                // Look Up Params
                //--------------------------------------------------------------
                const type_spec spec( typeid( typename ARRAY::type ) );
                const param    *p = params.search(spec);
                if(!p)
                    throw exception("no VisIt I/O for <'%s'>", spec.name());
                visit_handle h = VISIT_INVALID_HANDLE;
                
                //--------------------------------------------------------------
                // Allocate handle
                //--------------------------------------------------------------
                if(VisIt_VariableData_alloc(&h) != VISIT_OKAY)
                    throw exception("VisIt I/O::variable_data error");
                
                //--------------------------------------------------------------
                // prepare data
                //--------------------------------------------------------------
                const int nComponents= int(p->components);
                const int nTuples    = int(A.items);
                p->set_data(h,nComponents,nTuples,A.entry);
                return h;
            }
            
        private:
            inline void record( const param &p ) { (void) params.insert(p); }
            YOCTO_DISABLE_COPY_AND_ASSIGN(VisItIO);
            static void set_data_d(visit_handle &h,
                                   const int     nComponents,
                                   const int     nTuples,
                                   const void   *addr)
            {
                VisIt_VariableData_setDataD(h, VISIT_OWNER_SIM, nComponents, nTuples, (double*)addr);
            }
            
            static void set_data_f(visit_handle &h,
                                   const int     nComponents,
                                   const int     nTuples,
                                   const void   *addr)
            {
                VisIt_VariableData_setDataF(h, VISIT_OWNER_SIM, nComponents, nTuples, (float*)addr);
            }
            
            static void set_data_i(visit_handle &h,
                                   const int     nComponents,
                                   const int     nTuples,
                                   const void   *addr)
            {
                VisIt_VariableData_setDataI(h, VISIT_OWNER_SIM, nComponents, nTuples, (int*)addr);
            }
        };
        
    }
}


#endif
