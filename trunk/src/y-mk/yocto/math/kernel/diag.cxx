#include "yocto/math/kernel/diag.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/swap.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    
    namespace math
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // balancing algorithm
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        void diag<real_t>:: balance( matrix<real_t> &a ) throw()
        {
            static const real_t RADIX = REAL(2.0);
            
            assert( a.is_square() );
            const size_t n = a.rows;
            real_t s,r,g,f,c;
            
            const real_t sqrdx=RADIX*RADIX;
            size_t last=0;
            while(0==last)
            {
                last=1;
                for(size_t i=1;i<=n;i++)
                {
                    r=c=0.0;
                    for (size_t j=1;j<=n;j++)
                        if (j != i)
                        {
                            c += Fabs(a[j][i]);
                            r += Fabs(a[i][j]);
                        }
                    if (c && r)
                    {
                        g=r/RADIX;
                        f=1.0;
                        s=c+r;
                        while (c<g)
                        {
                            f *= RADIX;
                            c *= sqrdx;
                        }
                        g=r*RADIX;
                        while (c>g)
                        {
                            f /= RADIX;
                            c /= sqrdx;
                        }
                        if( (c+r)/f < 0.95*s)
                        {
                            last=0;
                            g=1.0/f;
                            for(size_t j=1;j<=n;j++) a[i][j] *= g;
                            for(size_t j=1;j<=n;j++) a[j][i] *= f;
                        }
                    }
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Hessenberg reduction
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        void diag<real_t>:: Hessenberg( matrix<real_t> &a ) throw()
        {
            assert( a.is_square() );
            const size_t n = a.rows;
            
            for(size_t m=2;m<n;m++)
            {
                real_t x=0;
                size_t i=m;
                
                for(size_t j=m;j<=n;j++)
                {
                    if (Fabs(a[j][m-1]) > Fabs(x))
                    {
                        x=a[j][m-1];
                        i=j;
                    }
                }
                
                if (i != m)
                {
                    for (size_t j=m-1;j<=n;j++) cswap(a[i][j],a[m][j]);
                    for (size_t j=1;j<=n;j++)   cswap(a[j][i],a[j][m]);
                }
                
                if(Fabs(x)>0)
                {
                    for(size_t i=m+1;i<=n;i++)
                    {
                        real_t y = a[i][m-1];
                        if( Fabs(y)>0 )
                        {
                            y /= x;
                            a[i][m-1]=y;
                            
                            for (size_t j=m;j<=n;j++)
                                a[i][j] -= y*a[m][j];
                            
                            for (size_t j=1;j<=n;j++)
                                a[j][m] += y*a[j][i];
                        }
                    }
                }
            }
            // clean up to th exat Hessenberg form
            for(size_t j=n;j>0;--j)
            {
                for(size_t i=j+2;i<=n;++i)
                    a[i][j] = 0;
            }
        }
        
        template <>
        void diag<real_t>:: eigenvalues( matrix<real_t> &a,
                                        array<real_t> &wr,
                                        array<real_t> &wi )
        {
            assert( a.is_square() );
            const size_t n = a.rows;
            assert( wr.size() >= n );
            assert( wi.size() >= n );
            int nn,m,l,k,j,its,i,mmin;
            float z,y,x,w,v,u,t,s,r,q,p,anorm;
            anorm=0.0;
            for (i=1;i<=n;i++)
                for (j=max_of(i-1,1);j<=n;j++) anorm += Fabs(a[i][j]);
            nn=n;
            t=0.0;
            
        }

        
    }
}
