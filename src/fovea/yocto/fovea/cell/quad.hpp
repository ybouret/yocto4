#ifndef YOCTO_FOVEA_CELL_QUAD_INCLUDED
#define YOCTO_FOVEA_CELL_QUAD_INCLUDED 1

#include "yocto/fovea/cell.hpp"

namespace yocto
{
    namespace fovea
    {
        void check_quad(const VertexBase &a,
                        const VertexBase &b,
                        const VertexBase &c,
                        const VertexBase &d);
        
        void no_possible_quad1D();
        void invalid_quad2D();

        template <size_t DIM, typename T>
        class Quad : public Cell<DIM,T>
        {
        public:
            typedef Vertex<DIM,T>        VERTEX;
            typedef Mesh<DIM,T>          MESH;
            typedef typename VERTEX::vtx vtx;
            
            explicit Quad(const VERTEX &a,
                          const VERTEX &b,
                          const VERTEX &c,
                          const VERTEX &d) :
            Cell<DIM,T>(4),
            area(0)
            {
                check_quad(a,b,c,d);
                this->p[0] = &a;
                this->p[1] = &b;
                this->p[2] = &c;
                this->p[3] = &d;
            }
            
            virtual ~Quad() throw()
            {
            }
            
            
            virtual void compile(const MESH &)
            {
                __compile( int2type<DIM>() );
            }
            
            const T  area;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Quad);
            
            inline void __compile( int2type<1> )
            {
                no_possible_quad1D();
            }
            
            inline void __compile( int2type<2> )
            {
                const int sa = this->signed_area( (T&)area );
                if(sa==0)
                    invalid_quad2D();
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

