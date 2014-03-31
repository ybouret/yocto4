#ifndef YOCTO_FOVEA_CELL_LINE_INCLUDED
#define YOCTO_FOVEA_CELL_LINE_INCLUDED 1

#include "yocto/fovea/cell.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_line(const VertexBase &a, const VertexBase &b);
        void check_edge(const void *addr,const edge_key &ek);
        
        template <size_t DIM,typename T>
        class Line : public Cell<DIM,T>
        {
        public:
            typedef Cell<DIM,T>           CELL;
            typedef typename CELL::VERTEX VERTEX;
            typedef typename CELL::EDGE   EDGE;
            typedef Mesh<DIM,T>           MESH;
            
            explicit Line(const VERTEX &a, const VERTEX &b) :
            Cell<DIM,T>(2),
            pEdge(0)
            {
                check_line(a,b);
                this->p[0] = &a;
                this->p[1] = &b;
            }
            
            virtual ~Line() throw()
            {
            }
            
            virtual void compile( const MESH &m )
            {
                const CELL &cell = *this;
                const edge_key ek(cell[0].index,cell[1].index);
                pEdge = m.edb.search(ek);
                check_edge(pEdge,ek);
                // the edge has a middle and a length...
            }
            
            inline const EDGE & edge() const throw()
            {
                assert(pEdge);
                return *pEdge;
            }
            
            
        private:
            const EDGE *pEdge;

            YOCTO_DISABLE_COPY_AND_ASSIGN(Line);
        };
        
    }
}

#endif
