#include "yocto/math/kernel/diag.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/swap.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/exception.hpp"

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
        
#define __CHECK_ROW(I) assert((I)>=1); assert((I)<=n)
        
        template <>
        void diag<real_t>:: eigenvalues(matrix<real_t> &a,
                                        array<real_t>  &wr,
                                        array<real_t>  &wi)
        {
            assert( a.is_square() );
            const size_t n = a.rows;
            assert( wr.size() >= n );
            assert( wi.size() >= n );
            
            int nn,m,l,k,j,its,i,mmin;
            float z,y,x,w,v,u,t,s,r,q,p,anorm;
            
            anorm=fabs(a[1][1]);
            for (i=2;i<=n;i++)
                for (j=(i-1);j<=n;j++)
                    anorm += fabs(a[i][j]);
            nn=n;
            t=0.0;
            while (nn >= 1) {
                its=0;
                do {
                    for (l=nn;l>=2;l--)
                    {
                        s=fabs(a[l-1][l-1])+fabs(a[l][l]);
                        if (s == 0.0) s=anorm;
                        if ((float)(fabs(a[l][l-1]) + s) == s) break;
                    }
                    x=a[nn][nn];
                    if (l == nn) {
                        wr[nn]=x+t;
                        wi[nn--]=0.0;
                    } else {
                        y=a[nn-1][nn-1];
                        w=a[nn][nn-1]*a[nn-1][nn];
                        if (l == (nn-1)) {
                            p=0.5*(y-x);
                            q=p*p+w;
                            z=sqrt(fabs(q));
                            x += t;
                            if (q >= 0.0) {
                                z=p+Signed(z,p);
                                wr[nn-1]=wr[nn]=x+z;
                                if (z) wr[nn]=x-w/z;
                                wi[nn-1]=wi[nn]=0.0;
                            } else {
                                wr[nn-1]=wr[nn]=x+p;
                                wi[nn-1]= -(wi[nn]=z);
                            }
                            nn -= 2;
                        } else {
                            if (its == 30) throw exception("Too many iterations in hqr");
                            if (its == 10 || its == 20) {
                                t += x;
                                for (i=1;i<=nn;i++) a[i][i] -= x;
                                s=fabs(a[nn][nn-1])+fabs(a[nn-1][nn-2]);
                                y=x=0.75*s;
                                w = -0.4375*s*s;
                            }
                            ++its;
                            for (m=(nn-2);m>=l;m--) {
                                z=a[m][m];
                                r=x-z;
                                s=y-z;
                                p=(r*s-w)/a[m+1][m]+a[m][m+1];
                                q=a[m+1][m+1]-z-r-s;
                                r=a[m+2][m+1];
                                s=fabs(p)+fabs(q)+fabs(r);
                                p /= s;
                                q /= s;
                                r /= s;
                                if (m == l) break;
                                u=fabs(a[m][m-1])*(fabs(q)+fabs(r));
                                v=fabs(p)*(fabs(a[m-1][m-1])+fabs(z)+fabs(a[m+1][m+1]));
                                if ((float)(u+v) == v) break;
                            }
                            for (i=m+2;i<=nn;i++) {
                                a[i][i-2]=0.0;
                                if (i != (m+2)) a[i][i-3]=0.0;
                            }
                            for (k=m;k<=nn-1;k++) {
                                if (k != m) {
                                    p=a[k][k-1];
                                    q=a[k+1][k-1];
                                    r=0.0;
                                    if (k != (nn-1)) r=a[k+2][k-1];
                                    if ((x=fabs(p)+fabs(q)+fabs(r)) != 0.0) {
                                        p /= x;
                                        q /= x;
                                        r /= x;
                                    }
                                }
                                if ((s=Signed(sqrt(p*p+q*q+r*r),p)) != 0.0) {
                                    if (k == m) {
                                        if (l != m)
                                            a[k][k-1] = -a[k][k-1];
                                    } else
                                        a[k][k-1] = -s*x;
                                    p += s;
                                    x=p/s;
                                    y=q/s;
                                    z=r/s;
                                    q /= p;
                                    r /= p;
                                    for (j=k;j<=nn;j++) {
                                        p=a[k][j]+q*a[k+1][j];
                                        if (k != (nn-1)) {
                                            p += r*a[k+2][j];
                                            a[k+2][j] -= p*z;
                                        }
                                        a[k+1][j] -= p*y;   
                                        a[k][j] -= p*x;   
                                    }   
                                    mmin = nn<k+3 ? nn : k+3;   
                                    for (i=l;i<=mmin;i++) {   
                                        p=x*a[i][k]+y*a[i][k+1];   
                                        if (k != (nn-1)) {   
                                            p += z*a[i][k+2];   
                                            a[i][k+2] -= p*r;   
                                        }   
                                        a[i][k+1] -= p*q;   
                                        a[i][k] -= p;   
                                    }   
                                }   
                            }   
                        }   
                    }   
                } while (l < nn-1);   
            }
            
#if 0
            size_t nn,m,l,k,j,its,i,mmin;
            real_t z,y,x,w,v,u,t,s,r,q,p,anorm;
            
            anorm=0;
            for (i=1;i<=n;i++)
            {
                for (j=max_of<size_t>(i-1,1);j<=n;j++)
                {
                    anorm += Fabs(a[i][j]);
                }
            }
            nn=n;
            t=0;
            while (nn >= 1)
            {
                std::cerr << "nn=" << nn << std::endl;
                its=0;
                do
                {
                    
                    for (l=nn;l>=2;l--)
                    {
                        std::cerr << "\tl=" << l << std::endl;
                        __CHECK_ROW(l-1);
                        __CHECK_ROW(l);
                        s=Fabs(a[l-1][l-1])+Fabs(a[l][l]);
                        if (s<=REAL(0.0) )
                            s=anorm;
                        if ((real_t)(Fabs(a[l][l-1]) + s) == s)
                        {
                            a[l][l-1]=0;
                            break;
                        }
                    }
                    std::cerr << "\t->l=" << l << "/nn=" << nn << std::endl;
                    __CHECK_ROW(nn);
                    x=a[nn][nn];
                    if (l == nn)
                    {
                        wr[nn]=x+t;
                        wi[nn]=0.0;
                        --nn;
                    }
                    else
                    {
                        __CHECK_ROW(nn-1);
                        y=a[nn-1][nn-1];
                        w=a[nn][nn-1]*a[nn-1][nn];
                        if (l == (nn-1))
                        {
                            p=REAL(0.5)*(y-x);
                            q=p*p+w;
                            z=Sqrt(Fabs(q));
                            x += t;
                            if (q >= 0.0)
                            {
                                z=p+Signed(z,p);
                                wr[nn-1]=wr[nn]=x+z;
                                if(z)
                                    wr[nn]=x-w/z;
                                wi[nn-1]=wi[nn]=0;
                            }
                            else
                            {
                                wr[nn-1]=wr[nn]=x+p;
                                wi[nn-1]= -(wi[nn]=z);
                            }
                            nn -= 2;
                        }
                        else
                        {
                            if (its == 30) throw exception("Too many iterations in hqr");
                            if (its == 10 || its == 20)
                            {
                                t += x;
                                for (i=1;i<=nn;i++)
                                    a[i][i] -= x;
                                __CHECK_ROW(nn-1);
                                s=Fabs(a[nn][nn-1])+Fabs(a[nn-1][nn-2]);
                                y=x=REAL(0.75)*s;
                                w = -REAL(0.4375)*s*s;
                            }
                            ++its;
                            for (m=(nn-2);m>=l;m--)
                            {
                                __CHECK_ROW(nn-2);
                                __CHECK_ROW(m);
                                z=a[m][m];
                                r=x-z;
                                s=y-z;
                                p=(r*s-w)/a[m+1][m]+a[m][m+1];
                                q=a[m+1][m+1]-z-r-s;
                                r=a[m+2][m+1];
                                s=Fabs(p)+Fabs(q)+Fabs(r);
                                p /= s;
                                q /= s;
                                r /= s;
                                if (m == l)
                                    break;
                                u=Fabs(a[m][m-1])*(Fabs(q)+Fabs(r));
                                v=Fabs(p)*(Fabs(a[m-1][m-1])+Fabs(z)+Fabs(a[m+1][m+1]));
                                if((real_t)(u+v)==v)
                                    break;
                            }
                            for(i=m+2;i<=nn;i++)
                            {
                                __CHECK_ROW(i);
                                a[i][i-2]=0.0;
                                if (i != (m+2))
                                    a[i][i-3]=0;
                            }
                            for(k=m;k<=nn-1;k++)
                            {
                                if (k != m)
                                {
                                    __CHECK_ROW(k);
                                    p=a[k][k-1];
                                    q=a[k+1][k-1];
                                    r=0.0;
                                    if (k != (nn-1))
                                    {
                                        r=a[k+2][k-1];
                                    }
                                    if ((x=Fabs(p)+Fabs(q)+Fabs(r)) != 0.0)
                                    {
                                        p /= x;
                                        q /= x;
                                        r /= x;
                                    }
                                }
                                if ((s=Signed(Sqrt(p*p+q*q+r*r),p)) != 0.0)
                                {
                                    if (k == m)
                                    {
                                        if (l != m)
                                        {
                                            __CHECK_ROW(k);
                                            a[k][k-1] = -a[k][k-1];
                                        }
                                    }
                                    else
                                    {
                                        __CHECK_ROW(k);
                                        a[k][k-1] = -s*x;
                                    }
                                    p += s;
                                    x=p/s;
                                    y=q/s;
                                    z=r/s;
                                    q /= p;
                                    r /= p;
                                    for(j=k;j<=nn;j++)
                                    {
                                        __CHECK_ROW(k);
                                        p=a[k][j]+q*a[k+1][j];
                                        if (k != (nn-1))
                                        {
                                            p += r*a[k+2][j];
                                            a[k+2][j] -= p*z;
                                        }
                                        a[k+1][j] -= p*y;
                                        a[k][j]   -= p*x;
                                    }
                                    mmin = nn<k+3 ? nn : k+3;
                                    for (i=l;i<=mmin;i++)
                                    {
                                        __CHECK_ROW(i);
                                        p=x*a[i][k]+y*a[i][k+1];
                                        if (k != (nn-1))
                                        {
                                            p += z*a[i][k+2];
                                            a[i][k+2] -= p*r;
                                        }
                                        a[i][k+1] -= p*q;
                                        a[i][k]   -= p;
                                    }
                                }
                            }
                        }
                    }
                }
                while (l<nn-1);
            }
#endif
        }
        
    }
}
