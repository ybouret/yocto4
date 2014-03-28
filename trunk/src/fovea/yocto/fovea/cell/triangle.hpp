#ifndef YOCTO_FOVEA_CELL_TRIANGLE_INCLUDED
#define YOCTO_FOVEA_CELL_TRIANGLE_INCLUDED 

#include "yocto/fovea/cell.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_triangle(const VertexBase &a, const VertexBase &b, const VertexBase &c);

        template <size_t DIM, typename T>
        class Triangle : public Cell<DIM,T>
        {
        public:
            typedef  Vertex<DIM,T> VERTEX;
            explicit Triangle(const VERTEX &a,
                              const VERTEX &b,
                              const VERTEX &c) :
            Cell<DIM,T>(3)
            {
                check_triangle(a,b,c);
                this->p[0] = &a;
                this->p[1] = &b;
                this->p[2] = &c;
            }
            
            virtual ~Triangle() throw()
            {
            }
            
            
            virtual void compile()
            {
                
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Triangle);
        };
        
    }
}

#endif

