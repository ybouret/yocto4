#ifndef YOCTO_FOVEA_LINE_INCLUDED
#define YOCTO_FOVEA_LINE_INCLUDED 1

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
                p[0] = &a;
                p[1] = &b;
            }
            
            virtual ~Line() throw()
            {
            }
            
            virtual void compile()
            {
                
            }
            
        private:
            const VERTEX *p[2];
            YOCTO_DISABLE_COPY_AND_ASSIGN(Line);
            virtual const VERTEX **handle() const throw()
            {
                return (const VERTEX **)p;
            }
        };
        
    }
}

#endif
