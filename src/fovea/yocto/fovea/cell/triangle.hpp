#ifndef YOCTO_FOVEA_CELL_TRIANGLE_INCLUDED
#define YOCTO_FOVEA_CELL_TRIANGLE_INCLUDED 1

#include "yocto/fovea/cell.hpp"

namespace yocto
{
    namespace fovea
    {
        
        struct TriangleInfo
        {
            static const char *Name() throw();
            static void CheckVertices( const VertexBase &a, const VertexBase &b, const VertexBase &c );
            static void NoEdgeFor( const VertexBase &a, const VertexBase &b );
            static void NoTriangle1D();
        };
        
        
        template <size_t DIM,typename T>
        class Triangle : public Cell<DIM,T>
        {
        public:
            typedef Cell<DIM,T>           CELL;
            typedef typename CELL::VERTEX VERTEX;
            typedef typename CELL::EDGE   EDGE;
            typedef typename CELL::MESH   MESH;
            typedef typename VERTEX::vtx  vtx;
            
            virtual ~Triangle() throw() {}
            
            explicit Triangle(const VERTEX &a, const VERTEX &b, const VERTEX &c ) :
            CELL(3)
            {
                TriangleInfo::CheckVertices(a, b, c);
                p[0] = &a;
                p[1] = &b;
                p[2] = &c;
            }
            
            inline virtual const char * name() const throw() { return TriangleInfo::Name(); }
            
            
            inline virtual void load_edges( const MESH &m )
            {
                __load(int2type<DIM>());
            }
            
            
        private:
            const VERTEX *p[3];
            const EDGE   *e[3];
            YOCTO_DISABLE_COPY_AND_ASSIGN(Triangle);
            virtual const VERTEX **ppVTX() const throw() { return (const VERTEX **)p; }
            
            //__________________________________________________________________
            //
            // 1D triangle: does not exist
            //__________________________________________________________________
            void __load( int2type<1> )
            {
                TriangleInfo:: NoTriangle1D();
            }
            
            //__________________________________________________________________
            //
            // 2D triangle: compute area
            //__________________________________________________________________
            static inline void update_signed_area( T &lhs, T &rhs, const vtx &i, const vtx &ip) throw()
            {
                lhs += i.x  * ip.y;
                rhs += ip.x * i.y;
            }
            
            void __load( int2type<2> )
            {
                //______________________________________________________________
                //
                // compute area with sign to make a CCW triangle
                //______________________________________________________________
                const vtx &p0 = p[0]->r;
                const vtx &p1 = p[1]->r;
                const vtx &p2 = p[2]->r;
                T lhs =0, rhs=0;
                update_signed_area(lhs, rhs, p0, p1);
                update_signed_area(lhs, rhs, p1, p2);
                update_signed_area(lhs, rhs, p2, p0);
                T &area = (T&)(this->size);
                if(lhs<rhs)
                {
                    // ClockWise
                    cswap(p[0],p[1]);
                    area = rhs - lhs;
                }
                else
                {
                    if(lhs>rhs)
                    {
                        // Counter ClockWise
                        area = lhs - rhs;
                    }
                    else
                        area = 0;
                }
                
            }
            
            //__________________________________________________________________
            //
            // 3D triangle: compute normal...
            //__________________________________________________________________
            void __load( int2type<3> )
            {
                
            }
        };
    }
    
}
#endif

