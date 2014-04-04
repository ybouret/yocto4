#ifndef YOCTO_FOVEA_SHAPE_INCLUDED
#define YOCTO_FOVEA_SHAPE_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/code/bzset.hpp"

namespace yocto
{
    namespace fovea
    {
        
        class ShapeBase : public object
        {
        public:
            static const size_t MIN_VERTICES = 2;
            static const size_t MAX_VERTICES = 4;
            virtual ~ShapeBase() throw();
            
            const size_t vertices; //!< vertices >= 2
            
            virtual const char *name() const throw() = 0;
            virtual size_t      index_of( size_t iv ) const throw() = 0;
            
        protected:
            explicit ShapeBase( size_t nv );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ShapeBase);
        };
        
        template <size_t DIM,typename T>
        class Shape : public ShapeBase
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef  Mesh<DIM,T>          MESH;
            typedef  Vertex<DIM,T>        VERTEX;
            typedef  typename VERTEX::vtx vtx;
            typedef  Edge<DIM,T>          EDGE;
            
            //__________________________________________________________________
            //
            // common data
            //__________________________________________________________________
            const T   size; //!< length/area/volume to be set by compile_for
            const vtx G;    //!< barycenter
         
            //__________________________________________________________________
            //
            // virtual API
            //__________________________________________________________________
            virtual ~Shape() throw() {}
            
            //! compute all data
            /**
             assuming that the physical edges are loaded,
             that all the edges are compiled.
             Must compute the barycenter and the size attribute.
             */
            virtual void   compile_for(const MESH &) = 0;
            virtual size_t index_of( size_t iv ) const throw()
            {
                assert(iv<vertices);
                const VERTEX **p = ppVTX();
                assert(p);
                assert(p[iv]!=0);
                return p[iv]->index;
            }

            
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
            
            inline void compute_barycenter() throw()
            {
                const Shape &shape = *this;
                vtx sum;
                bzset(sum);
                for(size_t i=0;i<vertices;++i)
                {
                    sum += shape[i].r;
                }
                T *g = (T*)&G;
                for(size_t j=0;j<DIM;++j)
                    g[j] /= vertices;
            }
            
        protected:
            explicit Shape(size_t nv) :
            ShapeBase(nv),
            size(0),
            G()
            {
            }
            
            virtual const VERTEX **ppVTX() const throw() = 0;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Shape);
        };
        
        
    }
}

#endif

