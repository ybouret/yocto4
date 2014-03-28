#ifndef YOCTO_FOVEA_CELL_LINE_INCLUDED
#define YOCTO_FOVEA_CELL_LINE_INCLUDED 1

#include "yocto/fovea/cell.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_line(const VertexBase &a, const VertexBase &b);
        
        template <size_t DIM,typename T>
        class Line : public Cell<DIM,T>
        {
        public:
            typedef Vertex<DIM,T> VERTEX;
            explicit Line(const VERTEX &a, const VERTEX &b) :
            Cell<DIM,T>(2)
            {
                check_line(a,b);
                this->p[0] = &a;
                this->p[1] = &b;
            }
            
            virtual ~Line() throw()
            {
            }
            
            virtual void compile()
            {
                
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Line);
        };
        
    }
}

#endif
