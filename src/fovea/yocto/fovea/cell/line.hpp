#ifndef YOCTO_FOVEA_CELL_LINE_INCLUDED
#define YOCTO_FOVEA_CELL_LINE_INCLUDED 1

#include "yocto/fovea/cell.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_line_vertices(const VertexBase &a, const VertexBase &b);
        void check_line_edge(const void *addr,const edge_key &ek);
        
        template <size_t DIM,typename T>
        class Line : public Cell<DIM,T>
        {
        public:
            typedef Cell<DIM,T>           CELL;
            typedef typename CELL::VERTEX VERTEX;
            typedef typename CELL::EDGE   EDGE;
            typedef Mesh<DIM,T>           MESH;
            
            explicit Line(const VERTEX &a, const VERTEX &b) :
            Cell<DIM,T>(2,1)
            {
                check_line_vertices(a,b);
                p[0] = &a;
                p[1] = &b;
            }
            
            virtual ~Line() throw()
            {
            }
            
            virtual void compile( const MESH &m )
            {
                const CELL &cell = *this;
                const edge_key ek(cell[0].index,cell[1].index);
                e[0] = m.edb.search(ek);
                check_line_edge(e[0],ek);
            }
            
            
            
        private:
            const VERTEX *p[2];
            const EDGE   *e[1];
            
            virtual const VERTEX **ppVTX() const throw() { return (const VERTEX **)p; }
            virtual const EDGE   **ppEDG() const throw() { return (const EDGE   **)e; }
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(Line);
        };
        
    }
}

#endif
