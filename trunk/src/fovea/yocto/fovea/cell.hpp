#ifndef YOCTO_FOVEA_CELL_INCLUDED
#define YOCTO_FOVEA_CELL_INCLUDED 1

#include "yocto/fovea/mesh.hpp"

namespace yocto
{
    namespace fovea
    {
        
        class Shape
        {
        public:
            static const size_t MIN_VERTICES = 2;
            static const size_t MAX_VERTICES = 4;
            virtual ~Shape() throw();
            
            const size_t vertices; //!< vertices >= 2
            
            virtual const char *name() const throw() = 0;
            
        protected:
            explicit Shape( size_t nv );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Shape);
        };
    
        template <size_t DIM,typename T>
        class Cell : public Shape
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef  Mesh<DIM,T>   MESH;
            typedef  Vertex<DIM,T> VERTEX;
            typedef  Edge<DIM,T>   EDGE;
            
            //__________________________________________________________________
            //
            // common data
            //__________________________________________________________________
            const T  size; //!< length/area/volume to be set by compile_for

            //__________________________________________________________________
            //
            // virtual API
            //__________________________________________________________________
            virtual ~Cell() throw() {}
            
            virtual  void load_edges(const MESH &) = 0;
            
            
            //__________________________________________________________________
            //
            // Non Virtual API
            //__________________________________________________________________
            inline const VERTEX & operator[]( size_t iv ) const throw()
            {
                assert(iv<vertices);
                const VERTEX **p = ppVTX();
                assert(p);
                assert(p[iv]!=0);
                return *p[iv];
            }
            
        protected:
            explicit Cell(size_t nv) :
            Shape(nv),
            size(0)
            {
            }
            
            virtual const VERTEX **ppVTX() const throw() = 0;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
        };
        
        
    }
}

#endif

