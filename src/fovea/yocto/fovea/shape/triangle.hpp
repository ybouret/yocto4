#ifndef YOCTO_FOVEA_SHAPE_TRIANGLE_INCLUDED
#define YOCTO_FOVEA_SHAPE_TRIANGLE_INCLUDED 1

#include "yocto/fovea/shape.hpp"

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
            static void MissingEdge( const edge_key &ek );
            static void ZeroLength( const VertexBase &a, const VertexBase &b );

        };
        
        
        template <size_t DIM,typename T>
        class Triangle : public Shape<DIM,T>
        {
        public:
            typedef Shape<DIM,T>                   SHAPE;
            typedef typename SHAPE::VERTEX         VERTEX;
            typedef typename SHAPE::EDGE           EDGE;
            typedef typename SHAPE::MESH           MESH;
            typedef typename VERTEX::vtx           vtx;
            typedef typename vertex_for<3,T>::type vtx3;
            
            virtual ~Triangle() throw() {}
            
            explicit Triangle(const VERTEX &a, const VERTEX &b, const VERTEX &c ) :
            SHAPE(3),
            p(),
            e(),
            en()
            {
                TriangleInfo::CheckVertices(a, b, c);
                p[0] = &a;
                p[1] = &b;
                p[2] = &c;
                e[0] = e[1] = e[2] = 0;
            }
            
            inline virtual const char * name() const throw() { return TriangleInfo::Name(); }
            
            
            inline virtual void load_edges( const MESH &m )
            {
                __load(int2type<DIM>());
                for(size_t i=0;i<3;++i)
                {
                    const VERTEX &v1 = *p[i];
                    const VERTEX &v2 = *p[(i+1)%3];
                    const edge_key ek(v1.index,v2.index);
                    e[i] = m.edges.search(ek);
                    if( !e[i] )
                        TriangleInfo:: MissingEdge(ek);
                }
                __normals( int2type<DIM>() );
            }
            
            
        private:
            const VERTEX *p[3];
            const EDGE   *e[3];
            const vtx     en[3]; //!< edge normals
        public:
            const vtx3    sn;    //!< surface normal
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Triangle);
            virtual const VERTEX **ppVTX() const throw() { return (const VERTEX **)p; }
            
            
            //__________________________________________________________________
            //
            // 1D triangle: does not exist
            //__________________________________________________________________
            inline void __load( int2type<1> )
            {
                TriangleInfo:: NoTriangle1D();
            }
            
            inline void __normals( int2type<1> )
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
            
            inline void __load( int2type<2> )
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
            
            inline void __normals( int2type<2> )
            {
                
                for(size_t i=0;i<3;++i)
                {
                    const VERTEX &v1  = *p[i];
                    const VERTEX &v2  = *p[(i+1)%3];
                    const vtx     vv  = v2.r - v1.r;
                    const T       len = e[i]->length;
                    if(len<=0)
                        TriangleInfo::ZeroLength(v1,v2);
                    vtx &n = (vtx &)(en[i]);
                    n.x =  vv.y/len;
                    n.y = -vv.x/len;
                }
                (T&)(sn.z) = 1;
            }
            
            //__________________________________________________________________
            //
            // 3D triangle: compute normal...
            //__________________________________________________________________
            inline void __load( int2type<3> )
            {
                
            }
            
            inline void __normals( int2type<3> )
            {
            }
        };
    }
    
}
#endif

