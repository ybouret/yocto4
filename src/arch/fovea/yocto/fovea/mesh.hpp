#ifndef YOCTO_FOVEA_MESH_INCLUDED
#define YOCTO_FOVEA_MESH_INCLUDED 1

#include "yocto/fovea/array-db.hpp"
#include "yocto/fovea/edge.hpp"
#include "yocto/type/spec.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        class ShapeBase;
        
        class mesh
        {
        public:
            enum real_type
            {
                use_float,  //!< use float vertices
                use_double  //!< use double vertices
            };
            
            enum form_type
            {
                is_rectilinear,
                is_curvilinear,
                is_point
            };
            
            virtual ~mesh() throw();
            
            const size_t          dims;     //!< may be different from layout dimensions
            const size_t          vertices; //!< #vertices
            const form_type       form;     //!<
            const real_type       real;     //!< float|double
            array_db             &adb;      //!< for memory
            const size_t          rsz;      //!< sizeof(real)
            const type_spec       rsp;      //!< typeid(float) | typedif(float)
            
            static real_type   sz2fp( const unsigned sz ); //!< size to real_type
            static const char *axis_name( size_t dim );
            static const char *form2name( form_type ) throw();
            const char        *form_name() const throw();
            void               throw_multiple_edges(size_t i1, size_t i2);
            const char        *real_name() const throw();
            
            //! abstract vertex
            virtual const void *get_vertex_address( size_t iv ) const throw() = 0;
            
            //! abstract cells
            virtual size_t num_cells() const throw() = 0;
            
            //! get first cell
            virtual const ShapeBase *get_first_cell() const throw() = 0;
            
        protected:
            explicit mesh(array_db       &a,
                          const size_t    d,
                          const size_t    nv,
                          const form_type f,
                          const size_t    s) throw();
            
            bool assigned; //!< for vtx cleanup
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(mesh);
            
        };
        
        template <size_t DIM,typename T>
        class Mesh : public mesh
        {
        public:
            static const size_t       DIMS = DIM;
            typedef T                 TYPE;
            typedef Vertex<DIM,T>     VERTEX;
            typedef Edge<DIM,T>       EDGE;
            typedef typename EDGE::DB EDGE_DB;
            
            inline VERTEX & operator[]( size_t v ) throw()
            {
                assert(v<vertices);
                assert(pvtx);
                assert(assigned);
                return pvtx[v];
            }
            
            inline const VERTEX & operator[]( size_t v ) const throw()
            {
                assert(v<vertices);
                assert(pvtx);
                return pvtx[v];
            }
            
            virtual const void *get_vertex_address( size_t iv ) const throw()
            {
                assert(iv<vertices);
                assert(pvtx);
                return &(pvtx[iv].r);
            }

            
            virtual ~Mesh() throw() {
                if(assigned)
                {
                    size_t i = vertices;
                    while(i>0)
                    {
                        pvtx[--i].~VERTEX();
                    }
                }
                memory::kind<memory_kind>::release_as<VERTEX>(pvtx,nvtx);
            }
            
            // once all physical coordinates are set
            inline void compile() const throw()
            {
                //______________________________________________________________
                //
                // load all physical coordinates
                //______________________________________________________________
                for(size_t i=0;i<vertices;++i)
                {
                    pvtx[i].load();
                }
                
                //______________________________________________________________
                //
                // compile all edges
                //______________________________________________________________
                for( typename EDGE_DB::const_iterator i = edges.begin(); i != edges.end(); ++i )
                {
                    const EDGE &edge = *i;
                    edge.compile();
                }
                
                //______________________________________________________________
                //
                // compile all cells
                //______________________________________________________________
                
            }
            
        protected:
            explicit Mesh(array_db     &a,
                          const size_t nv,
                          const form_type f
                          ) :
            mesh(a,DIM,nv,f,sizeof(T)),
            nvtx( vertices ),
            pvtx( memory::kind<memory_kind>::acquire_as<VERTEX>(nvtx) ),
            edges()
            {
            }
            
        private:
            size_t    nvtx;    //!< for memory
        protected:
            VERTEX   *pvtx; //!< for memory
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Mesh);
        public:
            EDGE_DB edges;
        };
    }
    
}

#endif
