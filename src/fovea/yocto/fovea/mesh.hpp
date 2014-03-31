#ifndef YOCTO_FOVEA_MESH_INCLUDED
#define YOCTO_FOVEA_MESH_INCLUDED 1

#include "yocto/fovea/array-db.hpp"
#include "yocto/fovea/edge.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        
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
            
            const size_t    dims;     //!< may be different from layout dimensions
            const size_t    vertices; //!< #vertices
            const form_type form;
            const real_type real;
            array_db       &adb;
            
            virtual size_t num_edges() const throw() = 0;
            
            static real_type   sz2fp( const unsigned sz ); //!< size to real_type
            static const char *axis_name( size_t dim );
            static const char *form2text( form_type ) throw();
            const char        *form_id() const throw();
            
        protected:
            explicit mesh(array_db       &a,
                          const size_t    d,
                          const size_t    nv,
                          const form_type f,
                          const size_t    s) throw();
            
            bool assigned; //!< for VTX cleanup
            void throw_multiple_edges(size_t i1, size_t i2);
            
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
                assert(vtx);
                assert(assigned);
                return vtx[v];
            }
            
            inline const VERTEX & operator[]( size_t v ) const throw()
            {
                assert(v<vertices);
                assert(vtx);
                assert(assigned);
                return vtx[v];
            }
            
            virtual ~Mesh() throw() {
                if(assigned)
                {
                    size_t i = vertices;
                    while(i>0)
                    {
                        vtx[--i].~VERTEX();
                    }
                }
                memory::kind<memory_kind>::release_as<VERTEX>(vtx,num);
            }
            
        protected:
            explicit Mesh(array_db     &a,
                          const size_t nv,
                          const form_type f
                          ) :
            mesh(a,DIM,nv,f,sizeof(T)),
            num( vertices ),
            vtx( memory::kind<memory_kind>::acquire_as<VERTEX>(num) ),
	    edb()
            {
            }
            
        private:
            size_t    num; //!< for memory
        protected:
            VERTEX   *vtx; //!< for memory
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Mesh);
        public:
            EDGE_DB edb;
            virtual size_t num_edges() const throw() { return edb.size(); }
        };
    }
    
}

#endif
