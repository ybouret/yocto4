#ifndef YOCTO_FOVEA_VERTEX_INCLUDED
#define YOCTO_FOVEA_VERTEX_INCLUDED 1

#include "yocto/fovea/types.hpp"
#include "yocto/math/v3d.hpp"

namespace yocto
{
    namespace fovea
    {
        
        class VertexBase
        {
        public:
            virtual ~VertexBase() throw();
            const size_t index;
            
        protected:
            VertexBase(size_t idx ) throw();
            VertexBase(const VertexBase &) throw();
        private:
            YOCTO_DISABLE_ASSIGN(VertexBase);
        };
        
        template <size_t DIM,typename T>
        class Vertex : public VertexBase
        {
        public:
            typedef typename vertex_for<DIM,T>::type vtx;
            
            explicit Vertex( size_t idx, T &cx) throw() :
            VertexBase(idx),
            r(),
            q()
            {
                q[0] = &cx;
            }
            
            explicit Vertex(size_t idx, T &cx, T &cy) throw() :
            VertexBase(idx),
            r(), q()
            {
                q[0] = &cx;
                q[1] = &cy;
            }
            
            explicit Vertex(size_t idx, T &cx, T &cy, T &cz) throw() :
            VertexBase(idx),
            r(), q()
            {
                q[0] = &cx;
                q[1] = &cy;
                q[2] = &cz;
            }
            
            inline Vertex( const Vertex &other ) throw() :
            VertexBase(other),
            r(other.r), q()
            {
                for(size_t i=0;i<DIM;++i) q[i] = other.q[i];
            }
            
            inline ~Vertex() throw() {}
            
            inline T       &x() throw()       { assert(q[0]!=0); return *q[0]; }
            inline const T &x() const throw() { assert(q[0]!=0); return *q[0]; }
            
            inline T       &y() throw()       { assert(q[1]!=0); return *q[1]; }
            inline const T &y() const throw() { assert(q[1]!=0); return *q[1]; }
            
            inline T       &z() throw()       { assert(q[2]!=0); return *q[2]; }
            inline const T &z() const throw() { assert(q[2]!=0); return *q[2]; }
            
            void load() const throw()
            {
                load( int2type<DIM>() );
            }
            
            const vtx r;
        private:
            T *q[DIM];
            YOCTO_DISABLE_ASSIGN(Vertex);
            
            inline
            void load( int2type<1> ) const throw()
            {
                (T &)r = x();
            }
            
            inline
            void load( int2type<2> ) const throw()
            {
                (T &)(r.x) = x();
                (T &)(r.y) = y();
            }
            
            inline
            void load( int2type<3> ) const throw()
            {
                (T &)(r.x) = x();
                (T &)(r.y) = y();
                (T &)(r.z) = z();
            }
            
        };
        
    }
}

#endif

