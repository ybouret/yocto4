#ifndef YOCTO_FOVEA_CELL_INCLUDED
#define YOCTO_FOVEA_CELL_INCLUDED 1

#include "yocto/fovea/mesh.hpp"

namespace yocto
{
    namespace fovea
    {
        
        size_t check_num_vertices( size_t nv );
        
        template <size_t DIM,typename T>
        class Cell
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef typename vertex_for<DIM,T>::type vertex;
            typedef Vertex<DIM,T>                    VERTEX;
            typedef Edge<DIM,T>                      EDGE;
            typedef Mesh<DIM,T>                      MESH;
            
            //__________________________________________________________________
            //
            // public data
            //__________________________________________________________________
            const size_t vertices;
            
            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________
            //! access
            inline const VERTEX & operator[](size_t iv) const throw()
            {
                assert(iv<vertices);
                assert(p);
                assert(p[iv]);
                return *p[iv];
            }

            //! once physical vertices have been set
            virtual void compile( const MESH & ) = 0;
            
            virtual ~Cell() throw()
            {
                object::operator delete(p,vertices * sizeof(VERTEX*));
            }
            
            
        protected:
            const VERTEX **p;
            
            inline Cell( size_t nv ) :
            vertices( check_num_vertices(nv) ),
            p(  (const VERTEX **)(object::operator new(vertices * sizeof(VERTEX*) )) )
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);

        };
        
    }
}

#endif
