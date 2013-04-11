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
                    const size_t      num;
                    const size_t      cnt;
                    const size_t      off;
                    vtx               out[3];
                    
                    explicit vproxy( const array<vtx> &v ) :
                    usr(v),
                    num(v.size()),
                    cnt(num+3),
                    off(num+1),
                    out()
                    {
                    }
                    
                    ~vproxy() throw() {}
                    
                    inline const vtx & operator[]( size_t i ) const throw()
                    {
                        assert(i>=1);
                        assert(i<=cnt);
                        return i <= num ? usr[i] : out[i-off];
                    }
                    
                    
                    iTriangle *create(size_t a, size_t b, size_t c) const
                    {
                        assert(a>0);assert(b>0);assert(c>0);
                        assert(a<=cnt);assert(b<=cnt);assert(c<=cnt);
                        
                        //const vproxy &self = *this;
                        iTriangle *tr = new iTriangle(a,b,c);
                        //const vtx &v1 = self[tr->i1];
                        //const vtx &v2 = self[tr->i2];
                        //const vtx &v3 = self[tr->i3];
                        
                        real_t &radius = tr->get<real_t,0>();
                        
                        radius = 0;
                        
                        return tr;
                        
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
                // Super Triangle
                //
                ////////////////////////////////////////////////////////////////
                
                {
                    //-- compute barycenter
                    vertex G(0,0);
                    for(size_t i=n;i>0;--i)
                        G += vertices[i];
                    G /= real_t(n);
                    
                    //-- compute max radius
                    real_t rho = 0;
                    for(size_t i=n;i>0;--i)
                    {
                        const vertex d(G,vertices[i]);
                        rho = max_of(rho,d.norm2());
                    }
                    rho = numeric<real_t>::minimum + 1.2 * Sqrt(rho);
                    {
                        ios::ocstream fp("circle.dat",false);
                        for(double theta=0; theta < 6.3; theta += 0.1)
                        {
                            const vertex v = G + rho * vertex( Cos(theta), Sin(theta) );
                            fp("%g %g\n", v.x, v.y);
                        }
                    }
                    
                    rho += rho;
                    proxy.out[0] = G + rho*vertex(0,1);
                    proxy.out[1] = G + rho*vertex( sqrt(3)/2,-0.5);
                    proxy.out[2] = G + rho*vertex(-sqrt(3)/2,-0.5);
                    {
                        ios::ocstream fp("bt.dat",false);
                        for(size_t i=0; i < 4; ++i )
                            fp("%g %g\n", proxy.out[i%3].x, proxy.out[i%3].y);
                    }
                   
                }
                
                ////////////////////////////////////////////////////////////////
                //
                // initialize with super triangle
                //
                ////////////////////////////////////////////////////////////////
                const size_t n1 = n+1, n2=n+2,n3=n+3;
                trlist.push_back( proxy.create(n1,n2,n3) );
                
                
                
                
            }
            
        }
    }
    
}
