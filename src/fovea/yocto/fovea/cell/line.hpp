#ifndef YOCTO_FOVEA_CELL_LINE_INCLUDED
#define YOCTO_FOVEA_CELL_LINE_INCLUDED 1

#include "yocto/fovea/cell.hpp"

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
        class Line : public Cell<DIM,T>
        {
        public:
            typedef Cell<DIM,T>           CELL;
            typedef typename CELL::VERTEX VERTEX;
            typedef typename CELL::EDGE   EDGE;
            typedef typename CELL::MESH   MESH;
            
            inline virtual ~Line() throw() {}
            
            inline explicit Line( const VERTEX &a, const VERTEX &b) :
            CELL(2),
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
                const CELL     &cell = *this;
                const edge_key  ek(cell[0].index,cell[1].index);
                const EDGE     *pE = m.edb.search(ek);
                if(!pE)
                    LineInfo::NoEdgeFor(cell[0],cell[1]);
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
