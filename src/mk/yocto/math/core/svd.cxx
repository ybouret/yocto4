#include "yocto/math/core/svd.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sequence/vector.hpp"
namespace yocto
{
    
    namespace math
    {
        /**
         Solves A·X = B for a vector X, where A is specified
         by the arrays u[1..m][1..n], w[1..n], v[1..n][1..n] as returned by svdcmp.
         m and n are the dimensions of a,
         and will be equal for square matrices.
         b[1..m] is the input right-hand side.
         x[1..n] is the output solution vector.
         No input quantities are destroyed,
         so the routine may be called sequentially with different b’s.
         */
        template <>
        void svd<real_t>::solve(const matrix<real_t> &u,
                                const array<real_t>  &w,
                                const matrix<real_t> &v,
                                const array<real_t>  &b,
                                array<real_t>        &x)
        {
            const size_t m = u.rows;
            const size_t n = u.cols;
            assert( w.size() == n );
            assert( v.rows == n);
            assert( v.cols == n );
            
            vector<real_t> tmp(n,0);
            for(size_t j=n;j>0;--j) {
                real_t s=0.0;
                if( Fabs(w[j])>0 )
                {
                    for(size_t i=m;i>0;--i) s += u[i][j]*b[i];
                    s /= w[j];
                }
                tmp[j]=s;
            }
            for(size_t j=n;j>0;--j)
            {
                real_t s=0.0;
                for(size_t jj=n;jj>0;--jj) s += v[j][jj]*tmp[jj];
                x[j]=s;
            }
        }
        
        
        static const size_t SVD_MAX_ITS = 1024;
        /**********************************************************************/
        template <>
        bool svd<real_t>:: build(matrix<real_t> &a, array<real_t> &w, matrix<real_t> &v)
        /***********************************************************************
         Given a matrix a[1..m][1..n], this routine computes its singular value
         decomposition, A = U.W.VT.  The matrix U replaces a on output.
         The diagonal matrix of singular values W is output as a vector w[1..n].
         The matrix V (not the transpose VT) is output as v[1..n][1..n].
         **********************************************************************/
        {
            const size_t m = a.rows;
            const size_t n = a.cols;
            assert(v.cols==a.cols);
            assert(v.rows==a.cols);
            assert(w.size()==a.cols);
            vector<real_t> rv1(n,0);
            
            size_t k;
            real_t c,f,h,s,x,y,z;
            size_t l = 0;
            real_t g=0,scale=0,anorm=0; /* Householder reduction to bidiagonal form */
            for(size_t i=1;i<=n;i++)
            {
                l=i+1;
                rv1[i]=scale*g;
                g=s=scale=0.0;
                if (i <= m)
                {
                    for(k=i;k<=m;k++)
                        scale += Fabs(a[k][i]);
                    if(scale>0)
                    {
                        for(k=i;k<=m;k++)
                        {
                            a[k][i] /= scale;
                            s += a[k][i]*a[k][i];
                        }
                        f = a[i][i];
                        g = -Signed(Sqrt(s),f);
                        h = f*g-s;
                        a[i][i]=f-g;
                        for(size_t j=l;j<=n;j++)
                        {
                            for(s=0.0,k=i;k<=m;k++)
                                s += a[k][i]*a[k][j];
                            f=s/h;
                            for(k=i;k<=m;k++)
                                a[k][j] += f*a[k][i];
                        }
                        for (k=i;k<=m;k++) a[k][i] *= scale;
                    }
                }
                w[i]=scale *g;
                g=s=scale=0.0;
                if (i <= m && i != n)
                {
                    for(k=l;k<=n;k++)
                        scale += Fabs(a[i][k]);
                    if (scale>0)
                    {
                        for(k=l;k<=n;k++)
                        {
                            a[i][k] /= scale;
                            s += a[i][k]*a[i][k];
                        }
                        f=a[i][l];
                        g = -Signed(Sqrt(s),f);
                        h=f*g-s;
                        a[i][l]=f-g;
                        for(k=l;k<=n;k++)
                            rv1[k]=a[i][k]/h;
                        for(size_t j=l;j<=m;j++) {
                            for (s=0.0,k=l;k<=n;k++)
                                s += a[j][k]*a[i][k];
                            for(k=l;k<=n;k++)
                                a[j][k] += s*rv1[k];
                        }
                        for (k=l;k<=n;k++)
                            a[i][k] *= scale;
                    }
                }
                anorm = max_of<real_t>(anorm,(Fabs(w[i])+Fabs(rv1[i])));
            }
            for(size_t i=n;i>=1;i--)
            {
                /* Accumulation of right-hand transformations. */
                if (i<n)
                {
                    if(Fabs(g)>0)
                    {
                        for(size_t j=l;j<=n;j++) /* double division to avoid possible underflow. */
                            v[j][i]=(a[i][j]/a[i][l])/g;
                        for(size_t j=l;j<=n;j++)
                        {
                            for (s=0.0,k=l;k<=n;k++)
                                s += a[i][k]*v[k][j];
                            for (k=l;k<=n;k++)
                                v[k][j] += s*v[k][i];
                        }
                    }
                    for(size_t j=l;j<=n;j++)
                        v[i][j]=v[j][i]=0.0;
                }
                v[i][i]=1.0;
                g=rv1[i];
                l=i;
            }
            for(size_t i=min_of(m,n);i>=1;i--)
            {
                /* Accumulation of left-hand transformations. */
                l=i+1;
                g=w[i];
                for(size_t j=l;j<=n;j++)
                    a[i][j]=0.0;
                if(Fabs(g)>0)
                {
                    g=real_t(1)/g;
                    for(size_t j=l;j<=n;j++)
                    {
                        for (s=0,k=l;k<=m;k++) s += a[k][i]*a[k][j];
                        f=(s/a[i][i])*g;
                        for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
                    }
                    for(size_t j=i;j<=m;j++) a[j][i] *= g;
                }
                else
                    for(size_t j=i;j<=m;j++) a[j][i]=0.0;
                ++a[i][i];
            }
            for (k=n;k>=1;k--)
            {
                /* Diagonalization of the bidiagonal form. */
                unsigned its=0;
                for(its=1;its<=SVD_MAX_ITS;its++)
                {
                    int   flag = 1;
                    size_t nm  = 0;
                    for (l=k;l>=1;l--)
                    {
                        /* Test for splitting. */
                        nm=l-1;
                        
                        /* Note that rv1[1] is always zero. */
                        if ((real_t)(Fabs(rv1[l])+anorm) == anorm)
                        {
                            flag=0;
                            break;
                        }
                        if ((real_t)(Fabs(w[nm])+anorm) == anorm) break;
                    }
                    if (flag)
                    {
                        c=0; /* Cancellation of rv1[l], if l > 1. */
                        s=1;
                        for(size_t i=l;i<=k;i++) {
                            f=s*rv1[i];
                            rv1[i]=c*rv1[i];
                            if ((real_t)(Fabs(f)+anorm) == anorm) break;
                            g=w[i];
                            h=Hypotenuse(f,g);
                            w[i]=h;
                            h=real_t(1)/h;
                            c=g*h;
                            s = -f*h;
                            for(size_t j=1;j<=m;j++) {
                                y=a[j][nm];
                                z=a[j][i];
                                a[j][nm]=y*c+z*s;
                                a[j][i]=z*c-y*s;
                            }
                        }
                    }
                    z=w[k];
                    if (l == k) { /* Convergence. */
                        if(z<0) { /* Singular value is made nonnegative. */
                            w[k] = -z;
                            for(size_t j=1;j<=n;j++) v[j][k] = -v[j][k];
                        }
                        break;
                    }
                    if (its >= SVD_MAX_ITS)
                    {
                        return false; // no convergence
                    }
                    x=w[l]; /* Shift from bottom 2-by-2 minor. */
                    nm=k-1;
                    y=w[nm];
                    g=rv1[nm];
                    h=rv1[k];
                    f=((y-z)*(y+z)+(g-h)*(g+h))/(real_t(2)*h*y);
                    g=Hypotenuse(f,real_t(1.0));
                    f=((x-z)*(x+z)+h*((y/(f+Signed(g,f)))-h))/x;
                    c=s=1.0; /* Next QR transformation: */
                    for(size_t j=l;j<=nm;j++)
                    {
                        const size_t i=j+1;
                        g=rv1[i];
                        y=w[i];
                        h=s*g;
                        g=c*g;
                        z=Hypotenuse(f,h);
                        rv1[j]=z;
                        c=f/z;
                        s=h/z;
                        f=x*c+g*s;
                        g=g*c-x*s;
                        h=y*s;
                        y *= c;
                        for(size_t jj=1;jj<=n;jj++)
                        {
                            x=v[jj][j];
                            z=v[jj][i];
                            v[jj][j]=x*c+z*s;
                            v[jj][i]=z*c-x*s;
                        }
                        z=Hypotenuse(f,h);
                        w[j]=z; /* Rotation can be arbitrary if z = 0. */
                        if(z>0) {
                            z=real_t(1)/z;
                            c=f*z;
                            s=h*z;
                        }
                        f=c*g+s*y;
                        x=c*y-s*g;
                        for(size_t jj=1;jj<=m;jj++)
                        {
                            y=a[jj][j];
                            z=a[jj][i];
                            a[jj][j]=y*c+z*s;
                            a[jj][i]=z*c-y*s;
                        }
                    }
                    rv1[l]=0.0;
                    rv1[k]=f;
                    w[k]=x;
                }
            }
            return true;
        }
        
        template <>
        size_t svd<real_t>:: truncate( array<real_t> &w, const real_t ftol )
        {
            size_t       ans = 0;
            const size_t n   = w.size();
            real_t wmax = 0;
            for( size_t i=n;i>0;--i)
            {
                const real_t tmp = Fabs( w[i] );
                if( tmp > wmax ) wmax = tmp;
            }
            const real_t wmin = Fabs( ftol * wmax );
            for( size_t i=n;i>0;--i)
            {
                if( Fabs(w[i]) <= wmin )
                {
                    w[i] = 0;
                    ++ans;
                }
            }
            return ans;
        }
        
        template <>
        bool svd<real_t>:: orthonormal(matrix<real_t>       &Q,
                                       const matrix<real_t> &P)
        {
            //------------------------------------------------------------------
            // sanity check
            //------------------------------------------------------------------
            assert(P.cols>0);
            assert(P.rows>0);
            assert(P.cols!=P.rows);
            
            //------------------------------------------------------------------
            // check dims
            //------------------------------------------------------------------
            const size_t nr  = P.rows;
            const size_t nc  = P.cols;
            size_t       dim = 0;
            size_t       sub = 0;
            const bool   cols_are_vector = nr>nc;
            if(cols_are_vector)
            {
                dim = nr;
                sub = nc;
            }
            else
            {
                dim = nc;
                sub = nr;
            }
            
            //------------------------------------------------------------------
            // Q space
            //------------------------------------------------------------------
            const size_t dof = dim-sub;
            if(cols_are_vector)
            {
                Q.make(dim,dof);
            }
            else
            {
                Q.make(dof,dim);
            }
            
            //------------------------------------------------------------------
            // total space
            //------------------------------------------------------------------
            matrix<real_t> F(dim,dim);
            if(cols_are_vector)
            {
                for(size_t v=sub;v>0;--v)
                {
                    for(size_t i=dim;i>0;--i)
                    {
                        F[i][v] = P[i][v];
                    }
                }
            }
            else
            {
                for(size_t v=sub;v>0;--v)
                {
                    for(size_t i=dim;i>0;--i)
                    {
                        F[i][v] = P[v][i];
                    }
                }
            }
            
            //------------------------------------------------------------------
            // use SVD
            //------------------------------------------------------------------
            {
                vector<real_t> W(dim,REAL(0.0));
                matrix<real_t> V(dim,dim);
                if(!svd<real_t>::build(F,W,V))
                    return false;
            }
            
            //------------------------------------------------------------------
            // save
            //------------------------------------------------------------------
            if(cols_are_vector)
            {
                for(size_t v=dof;v>0;--v)
                {
                    for(size_t i=dim;i>0;--i)
                    {
                        Q[i][v] = F[i][v+sub];
                    }
                }
            }
            else
            {
                for(size_t v=dof;v>0;--v)
                {
                    for(size_t i=dim;i>0;--i)
                    {
                        Q[v][i] = F[i][v+sub];
                    }
                }
                
            }
            
            // All done !
            return true;
        }
    }
}

