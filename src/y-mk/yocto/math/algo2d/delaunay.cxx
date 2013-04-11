#include "yocto/math/algo2d/delaunay.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/types.hpp"

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace algo2d
        {
            
            namespace
            {
                typedef geom::v2d<real_t> vtx;
                
                class vproxy
                {
                public:
                    const array<vtx> &usr;
                    const size_t      n;
                    const size_t      m;
                    const size_t      off;
                    vtx               out[3];
                    
                    explicit vproxy( const array<vtx> &v ) :
                    usr(v),
                    n(v.size()),
                    m(n+3),
                    off(n+1),
                    out()
                    {
                    }
                    
                    ~vproxy() throw() {}
                    
                    inline const vtx & operator[]( size_t i ) const throw()
                    {
                        assert(i>=1);
                        assert(i<=m);
                        return i <= n ? usr[i] : out[i-off];
                    }
                    
                    
                    iTriangle *create(size_t u, size_t v, size_t w) const
                    {
                        assert(u>0); assert(v>0); assert(w>0);
                        assert(u<=m);assert(v<=m);assert(w<=m);
                        
                        const vproxy &self = *this;
                        iTriangle *tr = new iTriangle(u,v,w);
                        const vtx &P1 = self[tr->i1];
                        const vtx &P2 = self[tr->i2];
                        const vtx &P3 = self[tr->i3];
                        
                        const vtx B(P1,P2);
                        const vtx C(P1,P3);
                        
                        
                        
                        const real_t D = 2*(B.x*C.y - B.y*C.x);
                        const real_t B2 = B.norm2();
                        const real_t C2 = C.norm2();
                        
                        vtx    &center = tr->get<vtx,1>();
                        
                        center.x = P1.x + (C.y * B2 - B.y * C2)/D;
                        center.y = P1.y + (B.x * C2 - C.x * B2)/D;
                        
                        
                        real_t &radius = tr->get<real_t,0>();
                        const vtx    tmp(P2,P3);
                        const real_t a = tmp.norm();
                        const real_t b = Sqrt(B2);
                        const real_t c = Sqrt(C2);
                        const real_t s = (a+b+c)/2;
                        
                        radius = REAL(0.25)*(a*b*c)/Sqrt( s * (s-a) * (s-b) * (s-c) );
                        {
                            ios::ocstream fp( vformat("tr%02u%02u%02u.dat", tr->i1, tr->i2, tr->i3), false );
                            fp( "%g %g\n", P1.x, P1.y);
                            fp( "%g %g\n", P2.x, P2.y);
                            fp( "%g %g\n", P3.x, P3.y);
                            fp( "%g %g\n", P1.x, P1.y);
                        }
                        
                        {
                            ios::ocstream fp( vformat("cc%02u%02u%02u.dat", tr->i1, tr->i2, tr->i3), false );
                            for( double theta=0; theta<=6.3; theta += 0.1 )
                            {
                                const vtx Q = center + radius * vtx( Cos(theta), Sin(theta) );
                                fp( "%g %g\n", Q.x, Q.y);
                            }
                        }
                        radius *= radius; //store r2
                        
                        return tr;
                    }
                    
                    
                    inline void initialize( iTriangle::List &trlist )
                    {
                        //======================================================
                        //-- compute barycenter
                        //======================================================
                        
                        vtx G(0,0);
                        for(size_t i=n;i>0;--i)
                            G += usr[i];
                        G /= real_t(n);
                        
                        //======================================================
                        //-- compute max radius
                        //======================================================
                        real_t rho = 0;
                        for(size_t i=n;i>0;--i)
                        {
                            const vtx d(G,usr[i]);
                            rho = max_of(rho,d.norm2());
                        }
                        rho = numeric<real_t>::minimum + 1.2 * Sqrt(rho);
                      
                        
                        //======================================================
                        // compute triangle vertices
                        //======================================================
                        static const vtx v_up(0,1);
                        static const vtx v_right(sqrt(3)/2,-0.5);
                        static const vtx v_left(-sqrt(3)/2,-0.5);
                        
                        rho += rho;
                        out[0] = G + rho* v_up;
                        out[1] = G + rho* v_right;
                        out[2] = G + rho* v_left;
                        {
                            ios::ocstream fp("bt.dat",false);
                            for(size_t i=0; i < 4; ++i )
                                fp("%g %g\n", out[i%3].x, out[i%3].y);
                        }
                        
                        //======================================================
                        // ok...
                        //======================================================
                        const size_t n1 = n+1, n2=n+2,n3=n+3;
                        trlist.push_back( create(n1,n2,n3) );
                    }
                    
                    inline void build( iTriangle::List &trlist )
                    {
                        assert( 1 == trlist.size );
                        iEdge::Buffer edges(32,as_capacity);
                        
                        for(size_t i=1; i <= n; ++i )
                        {
                            const vtx &v = usr[i];
                            //--------------------------------------------------
                            // eliminate triangles that encompass the vertex
                            // while storing the edges
                            //--------------------------------------------------
                            edges.free();
                            iTriangle::List tmp;
                            
                            while(trlist.size>0)
                            {
                                iTriangle    *tr = trlist.pop_front();
                                const real_t &r2 = tr->get<real_t,0>();
                                const vtx    &tc = tr->get<vtx,1>();
                                
                                const vtx     d(tc,v);
                                if( d.norm2() <= r2 )
                                {
                                    tr->insert_into(edges);
                                    delete tr;
                                }
                                else
                                    tmp.push_back(tr);
                            }
                            trlist.swap_with(tmp);
                            
                            //--------------------------------------------------
                            // build the new triangles
                            //--------------------------------------------------
                            std::cerr << "With point #" << i << std::endl;
                            for( size_t j=1; j <= edges.size(); ++j)
                            {
                                const iEdge &edge = edges[j];
                                std::cerr << "\t edge@" << edge.i1 << "," << edge.i2 << std::endl;
                                iTriangle  *tr    = create(i,edge.i1,edge.i2);
                                trlist.push_back(tr);
                            }
                            //if(i>=2) break;
                        }
                    }
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(vproxy);
                };
            }
            
            template <>
            void delaunay<real_t>:: build( iTriangle::List &trlist, const array<vertex> &vertices)
            {
                trlist.kill();
                const size_t n = vertices.size();
                if( n < 3 )
                    return;
                
                vproxy proxy(vertices);
                
                
                ////////////////////////////////////////////////////////////////
                //
                // initialize with super triangle
                //
                ////////////////////////////////////////////////////////////////
                proxy.initialize(trlist);
                
                ////////////////////////////////////////////////////////////////
                //
                // algorithm
                //
                ////////////////////////////////////////////////////////////////
                proxy.build(trlist);
                
                {
                    ios::ocstream fp("delaunay.dat",false);
                    for( const iTriangle *tr = trlist.head;tr;tr=tr->next)
                    {
                        const vtx &P1 = proxy[tr->i1];
                        const vtx &P2 = proxy[tr->i2];
                        const vtx &P3 = proxy[tr->i3];
                        fp( "%g %g\n", P1.x, P1.y);
                        fp( "%g %g\n", P2.x, P2.y);
                        fp( "%g %g\n", P3.x, P3.y);
                        fp( "%g %g\n", P1.x, P1.y);
                        fp("\n");
                    }
                }

            }
            
        }
    }
    
}
