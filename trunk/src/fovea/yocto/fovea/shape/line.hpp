#ifndef YOCTO_FOVEA_SHAPE_LINE_INCLUDED
#define YOCTO_FOVEA_SHAPE_LINE_INCLUDED 1

#include "yocto/fovea/shape.hpp"

namespace yocto
{
    namespace fovea
    {
     
        struct LineInfo
        {
            static const char *Name() throw();
            static void CheckVertices( const VertexBase &a, const VertexBase &b );
            static void NoEdgeFor( const VertexBase &a, const VertexBase &b );

        };
        
        template <size_t DIM, typename T>
        class Line : public Shape<DIM,T>
        {
        public:
            typedef Shape<DIM,T>           SHAPE;
            typedef typename SHAPE::VERTEX VERTEX;
            typedef typename SHAPE::EDGE   EDGE;
            typedef typename SHAPE::MESH   MESH;
            
            inline virtual ~Line() throw() {}
            
            inline explicit Line( const VERTEX &a, const VERTEX &b) :
            SHAPE(2),
            p(),
            e(0)
            {
                LineInfo::CheckVertices(a,b);
                p[0] = &a;
                p[1] = &b;
            }
            
            
            inline virtual const char * name() const throw() { return LineInfo::Name(); }
            
            inline virtual void load_edges( const MESH &m )
            {
                const SHAPE    &shape = *this;
                const edge_key  ek(shape[0].index,shape[1].index);
                const EDGE     *pE = m.edb.search(ek);
                if(!pE)
                    LineInfo::NoEdgeFor(shape[0],shape[1]);
                e = pE;
                (T&)(this->size) = e->length;
            }
            
        
        private:
            const VERTEX *p[2];
            const EDGE   *e;
            virtual const VERTEX **ppVTX() const throw() { return (const VERTEX **)p; }

            YOCTO_DISABLE_COPY_AND_ASSIGN(Line);
        };
        
        
    }
}

#endif
