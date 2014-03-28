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
            r()
            {
                r[0] = &cx;
            }
            
            explicit Vertex(size_t idx, T &cx, T &cy) throw() :
            VertexBase(idx),
            r()
            {
                r[0] = &cx;
                r[1] = &cy;
            }
            
            explicit Vertex(size_t idx, T &cx, T &cy, T &cz) throw() :
            VertexBase(idx),
            r()
            {
                r[0] = &cx;
                r[1] = &cy;
                r[2] = &cz;
            }
            
            inline Vertex( const Vertex &other ) throw() :
            VertexBase(other),
            r()
            {
                for(size_t i=0;i<DIM;++i) r[i] = other.r[i];
            }
            
            inline ~Vertex() throw() {}
            
            inline T       &x() throw()       { assert(r[0]!=0); return *r[0]; }
            inline const T &x() const throw() { assert(r[0]!=0); return *r[0]; }
            
            inline T       &y() throw()       { assert(r[1]!=0); return *r[1]; }
            inline const T &y() const throw() { assert(r[1]!=0); return *r[1]; }
            
            inline T       &z() throw()       { assert(r[2]!=0); return *r[2]; }
            inline const T &z() const throw() { assert(r[2]!=0); return *r[2]; }
            
            inline vtx make_pos() const throw()
            {
                return pos( int2type<DIM>() );
            }
            
        private:
            T *r[DIM];
            YOCTO_DISABLE_ASSIGN(Vertex);
            
            inline vtx pos( int2type<1> ) const throw()
            {
                return x();
            }
            
            inline vtx pos( int2type<2> ) const throw()
            {
                return vtx(x(),y());
            }
            
            inline vtx pos( int2type<3> ) const throw()
            {
                return vtx(x(),y(),z());
            }
        };
        
    }
}

#endif

