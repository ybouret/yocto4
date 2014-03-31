#ifndef YOCTO_FOVEA_CELL_TRIANGLE_INCLUDED
#define YOCTO_FOVEA_CELL_TRIANGLE_INCLUDED

#include "yocto/fovea/cell.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_triangle(const VertexBase &a, const VertexBase &b, const VertexBase &c);
        void no_possible_triangle1D();
        void invalid_triangle2D();
        
        template <size_t DIM, typename T>
        class Triangle : public Cell<DIM,T>
        {
        public:
            typedef Vertex<DIM,T>        VERTEX;
            typedef Mesh<DIM,T>          MESH;
            typedef typename VERTEX::vtx vtx;
            
            explicit Triangle(const VERTEX &a,
                              const VERTEX &b,
                              const VERTEX &c) :
            Cell<DIM,T>(3),
            area(0)
            {
                check_triangle(a,b,c);
                this->p[0] = &a;
                this->p[1] = &b;
                this->p[2] = &c;
            }
            
            virtual ~Triangle() throw()
            {
            }
            
            
            virtual void compile(const MESH &)
            {
                __compile( int2type<DIM>() );
            }
            
            const T   area;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Triangle);
            inline void __compile( int2type<1> )
            {
                no_possible_triangle1D();
            }
            
            inline void __compile( int2type<2> )
            {
                const int sa = this->signed_area( (T&)area );
                if(sa==0)
                    invalid_triangle2D();
                if(sa<0)
                {
                    cswap<const VERTEX *>( this->p[0], this->p[1]);
                }
                assert(area>=0);
            }
            
            inline void __compile( int2type<3> )
            {
                
            }
            
            
        };
        
    }
}

#endif

