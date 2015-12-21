#include "yocto/math/core/diag.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"


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
        void diag<real_t>:: HessenbergBalance( matrix<real_t> &a ) throw()
        {
            static const real_t RADIX = REAL(2.0);
            static const real_t sqrdx = RADIX*RADIX;
            
            assert( a.is_square() );
            const size_t n = a.rows;
            
            size_t last=0;
            while(0==last)
            {
                last=1;
                for(size_t i=1;i<=n;i++)
                {
                    real_t r=0,c=0;
                    for (size_t j=1;j<=n;j++)
                        if (j != i)
                        {
                            c += Fabs(a[j][i]);
                            r += Fabs(a[i][j]);
                        }
                    if( (c>0) && (r>0) )
                    {
                        real_t g=r/RADIX;
                        real_t f=REAL(1.0);
                        real_t s=c+r;
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
                        if( (c+r)/f < REAL(0.95)*s)
                        {
                            last=0;
                            g=REAL(1.0)/f;
                            for(size_t j=1;j<=n;j++) a[i][j] *= g;
                            for(size_t j=1;j<=n;j++) a[j][i] *= f;
                        }
                    }
                }
            }
        }
        
#if 0
        template <>
        void diag<real_t>:: HessenbergReOrder( matrix<real_t> &a ) throw()
        {
            //std::cerr << "// Hessenberg ReOrder" << std::endl;
            assert( a.is_square() );
            const size_t n = a.rows;
            for(size_t i=1; i <= n; ++i )
            {
                real_t piv = Fabs(a[i][i]);
                size_t m   = i;
                for(size_t j=i+1; j<=n;++j)
                {
                    const real_t tmp = Fabs(a[j][j]);
                    if(tmp>piv)
                    {
                        piv=tmp;
                        m  = j;
                    }
                }
                if(i!=m)
                    a.swap_both(i,m);
            }
        }
#endif
        
    }
}


#include "yocto/code/bswap.hpp"

namespace yocto {
    
    namespace math {
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Hessenberg reduction
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        void diag<real_t>:: HessenbergReduce( matrix<real_t> &a ) throw()
        {
            assert(a.is_square());
            const size_t n = a.rows;
            
            //------------------------------------------------------------------
            // m = r+1
            //------------------------------------------------------------------
            for(size_t m=2; m<n; ++m )
            {
                const size_t r   = m-1;
                real_t       piv = 0;
                size_t       s   = m;
                for( size_t j=m+1;j<=n;++j)
                {
                    //----------------------------------------------------------
                    // find the pivot
                    //----------------------------------------------------------
                    const real_t tmp = a[j][r];
                    if(Fabs(tmp)>Fabs(piv))
                    {
                        piv = tmp;
                        s   = j;
                    }
                }
                if( s != m )
                {
                    //std::cerr << "\t#SWAP(" << s << "," << m << ")" <<  "/pivot=" << piv << std::endl;
                    assert(Fabs(piv)>0);
                    //----------------------------------------------------------
                    // First similarity transform: exchange colums/rows
                    //----------------------------------------------------------
                    a.swap_both(s,m);
                    
                    //----------------------------------------------------------
                    // Second similarity transform
                    //----------------------------------------------------------
                    assert( Fabs(piv-a[m][m-1]) <= 0 );
                    for(size_t i=m+1;i<=n;++i)
                    {
                        const real_t factor = a[i][r] / piv;
                        
                        //------------------------------------------------------
                        // subtract factor times row r + 1 from row i
                        //------------------------------------------------------
                        for(size_t j=1;j<=n;++j) a[i][j] -= factor * a[m][j];
                        
                        //------------------------------------------------------
                        // add factor times column i to column r + 1
                        //------------------------------------------------------
                        for(size_t j=1;j<=n;j++) a[j][m] += factor * a[j][i];
                    }
                }
            }
            
            //==================================================================
            // clean up to the exact Hessenberg form
            //==================================================================
            for(size_t j=n;j>0;--j)
            {
                for(size_t i=j+2;i<=n;++i)
                    a[i][j] = 0;
            }
            
        }
        
    }
}


#include "yocto/code/utils.hpp"
#include "yocto/sort/heap.hpp"

namespace yocto
{
    
    namespace math
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Hessenberg QR for eigenvalues
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        bool diag<real_t>:: HessenbergQR(matrix<real_t> &a,
                                         array<real_t>  &wr,
                                         array<real_t>  &wi,
                                         size_t         &nr) throw()
        {
            assert( a.is_square() );
            const ptrdiff_t n = a.rows;
            assert( wr.size()   >= a.rows );
            assert( wi.size()   >= a.rows );
            //assert( flag.size() >= n );
            
            ptrdiff_t nn,m,l,k,j,i,mmin;
            real_t    z,y,x,w,v,u,t,s,r=0,q=0,p=0,anorm;
            
            size_t   ir = 1; //! where to put real eigenvalues
            size_t   ic = n; //! where to put cplx eigenvalues
            nr = 0;
            anorm=0;
            for (i=1;i<=n;i++)
                for (j=max_of<size_t>(i-1,1);j<=n;j++)
                    anorm += Fabs(a[i][j]);
            nn=n;
            t=0;
            while(nn>=1)
            {
                unsigned its=0;
                do
                {
                    for (l=nn;l>=2;l--)
                    {
                        s=Fabs(a[l-1][l-1])+Fabs(a[l][l]);
                        if (s <= 0)
                            s=anorm;
                        if ((real_t)(Fabs(a[l][l-1]) + s) == s)
                            break;
                    }
                    x=a[nn][nn];
                    if (l == nn)
                    {
                        wr[ir]=x+t;
                        wi[ir]=0;
                        //std::cerr << "#EIG: real single: " << wr[ir] << std::endl;
                        ++ir;
                        ++nr;
                        --nn;
                    }
                    else
                    {
                        y=a[nn-1][nn-1];
                        w=a[nn][nn-1]*a[nn-1][nn];
                        if(l == (nn-1))
                        {
                            p=REAL(0.5)*(y-x);
                            q=p*p+w;
                            z=Sqrt(Fabs(q));
                            x += t;
                            if (q >= 0)
                            {
                                z=p+Signed(z,p);
                                wr[ir+1]=wr[ir]=x+z;
                                if( Fabs(z)>0 )
                                    wr[ir]=x-w/z;
                                //std::cerr << "#EIG: real pair: " << wr[ir] << ", " << wr[ir+1] << ", x=" << x << ", w=" << w << ", z=" << z << ", p=" << p << ", sq=" << Sqrt(Fabs(q)) << std::endl;
                                wi[ir+1]=wi[ir]=0;
                                ir += 2;
                                nr += 2;
                            }
                            else
                            {
                                wr[ic-1]=wr[ic]=x+p;
                                wi[ic-1]= -(wi[ic]=z);
                                ic -= 2;
                            }
                            nn -= 2;
                        }
                        else
                        {
                            if (its == 30)
                                return false;
                            if (0 == (its%10) )
                            {
                                t += x;
                                for (i=1;i<=nn;i++)
                                    a[i][i] -= x;
                                s=Fabs(a[nn][nn-1])+Fabs(a[nn-1][nn-2]);
                                y=x= REAL(0.75)*s;
                                w = -REAL(0.4375)*s*s;
                            }
                            ++its;
                            for(m=(nn-2);m>=l;m--)
                            {
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
                                if ((real_t)(u+v) == v)
                                    break;
                            }
                            for (i=m+2;i<=nn;i++)
                            {
                                a[i][i-2]=0;
                                if (i != (m+2))
                                    a[i][i-3]=0;
                            }
                            for (k=m;k<=nn-1;k++)
                            {
                                if (k != m)
                                {
                                    p=a[k][k-1];
                                    q=a[k+1][k-1];
                                    r=0;
                                    if (k != (nn-1)) r=a[k+2][k-1];
                                    if ( (x=Fabs(p)+Fabs(q)+Fabs(r))>0 )
                                    {
                                        p /= x;
                                        q /= x;
                                        r /= x;
                                    }
                                }
                                if( Fabs(s=Signed(Sqrt(p*p+q*q+r*r),p)) > 0 )
                                {
                                    if (k == m)
                                    {
                                        if (l != m)
                                            a[k][k-1] = -a[k][k-1];
                                    }
                                    else
                                        a[k][k-1] = -s*x;
                                    p += s;
                                    x=p/s;
                                    y=q/s;
                                    z=r/s;
                                    q /= p;
                                    r /= p;
                                    for(j=k;j<=nn;j++)
                                    {
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
            if(nr>0)
            {
                hsort(&wr[1], nr, __compare<real_t>);
            }
            return true;
        }
        
    }
    
}


#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/core/svd.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/exception.hpp"
#include "yocto/sort/index.hpp"

namespace yocto {
    
    namespace math
    {
        
        
        
        static inline int __compare_fabs( const real_t lhs, const real_t rhs) throw()
        {
            return __compare<real_t>( Fabs(lhs), Fabs(rhs) );
        }
        
        
        static inline
        real_t __refine(const matrix<real_t>  &A,
                        const matrix<real_t>  &B,
                        array<real_t>         &y,
                        array<real_t>         &z,
                        array<real_t>         &r
                        )
        {
            tao::mul(r, B, y);
            tao::mul(z, A, y);
            real_t zy  = 0;
            real_t ry  = 0;
            real_t den = 0;
            for(size_t i=y.size();i>0;--i)
            {
                zy += z[i] * y[i];
                ry += r[i] * y[i];
                den += y[i] * y[i];
            }
            return (zy+ry)/den;
        }
        
        template <>
        void diag<real_t>:: eigv(matrix<real_t>       &ev,
                                 const matrix<real_t> &A,
                                 array<real_t>        &wr )
        {
            assert(A.is_square());
            assert( wr.size() >= ev.rows );
            
            const size_t n  = A.rows;
            const size_t nv = ev.rows;
            assert(ev.cols==n);
            
            matrix<real_t> B(n,n); //! A - tau * Id
            matrix<real_t> U(n,n); //! for SVD
            matrix<real_t> V(n,n); //! for SVD
            vector<real_t> W(n,numeric<real_t>::zero); //! for SVD
            vector<size_t> J(n,0); //! for indexing |W|
            
            vector<real_t> y(n,numeric<real_t>::zero);
            vector<real_t> z(n,numeric<real_t>::zero);
            
            if( nv > 0 )
            {
                hsort(&wr[1], nv, __compare<real_t>);
            }
            
            ev.ldz();
            
            for(size_t iv=1; iv <= nv; )
            {
                //==============================================================
                //
                // Check null space in SVD
                //
                //==============================================================
                size_t nz = 0;
                while(true)
                {
                    //----------------------------------------------------------
                    // B = A - wr[iv]*Id
                    //----------------------------------------------------------
                    B.assign(A);
                    for(size_t i=n;i>0;--i)
                    {
                        B[i][i] -= wr[iv];
                    }
                    
                    //----------------------------------------------------------
                    // B = U * W * V'
                    //----------------------------------------------------------
                    U.assign(B);
                    if( !svd<real_t>::build(U, W, V) )
                    {
                        throw exception("diag::eigv(Bad Matrix,level-1)");
                    }
                    nz = svd<real_t>::truncate(W);
                    make_index(W, J, __compare_fabs);
                    
                    if(nz>0)
                        break;
                    
                    //----------------------------------------------------------
                    // inverse power using the smallest singular value
                    //----------------------------------------------------------
                    const size_t j0 = J[1]; assert(j0>0); assert(j0<=n);
                    for(size_t i=n;i>0;--i)
                        y[i] = V[i][j0];
                    svd<real_t>::solve(U, W, V, y, z);
                    
                    //----------------------------------------------------------
                    // improve tau
                    //----------------------------------------------------------
                    const real_t dtau = REAL(1.0) / tao::dot(y,z);
                    wr[iv] += dtau;
                }
                
                //==============================================================
                // Compute nullspace
                //==============================================================
                assert(nz>0);
                const real_t tau = wr[iv];
                for(size_t k=1;k<=nz;++k)
                {
                    if(iv>nv) break;
                    wr[iv] = tau;
                    array<real_t> &vec = ev[iv];
                    const size_t j = J[k]; assert(j>0); assert(j<=n);
                    assert(Fabs(W[j])<=0);
                    for(size_t i=n;i>0;--i)
                        vec[i] = V[i][j];
                    ++iv;
                }
            }
        }
        
        
        
    }
}
