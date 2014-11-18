#include "yocto/math/ztype.hpp"
#include "yocto/math/sparse/cj.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/sparse/algebra.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

namespace yocto
{
    
    namespace math
    {
        
        typedef sp_algebra<real_t> skl;
        
        template <>
        sp_cgrad<real_t>:: ~sp_cgrad() throw()
        {
            
        }
        
        template <>
        sp_cgrad<real_t>:: sp_cgrad() throw() :
        g(),h(),xi(),z(),r()
        {
            
        }
        
        template <>
        sp_cgrad<real_t>:: sp_cgrad(size_t n) :
        g(n,as_capacity),
        h(n,as_capacity),
        xi(n,as_capacity),
        z(n,as_capacity),
        q(n,as_capacity),
        r(n,as_capacity)
        {
            
        }
        
        namespace
        {
            static inline
            real_t Phi(const sp_matrix<real_t> &A,
                       const array<real_t>     &p,
                       const array<real_t>     &b,
                       array<real_t>           &grad,
                       array<real_t>           &r)
            {
                // residual
                tao::neg(r,b);
                skl::muladd(r,A,p);
                
                // gradient
                skl::mul_trn(grad, A, r);
                
                // phi
                real_t ans = REAL(0.0);
                for(size_t i=r.size();i>0;--i)
                {
                    const real_t xx  = r[i];
                    ans += xx*xx;
                }
                return REAL(0.5) * ans;
            }

            class spcj
            {
            public:
                inline ~spcj() throw() {}
                inline  spcj(const sp_matrix<real_t> &Aref,
                             const array<real_t>     &bref,
                             const array<real_t>     &pref,
                             array<real_t>           &qref,
                             const array<real_t>     &xiref,
                             array<real_t>           &zref,
                             array<real_t>           &rref) throw() :
                A(Aref),
                b(bref),
                p(pref),
                q(qref),
                xi(xiref),
                z(zref),
                r(rref)
                {
                }
                
                const sp_matrix<real_t> &A;
                const array<real_t>     &b;
                const array<real_t>     &p;
                array<real_t>           &q;
                const array<real_t>     &xi;
                array<real_t>           &z;
                array<real_t>           &r;
                
                real_t phi( real_t lam )
                {
                    for(size_t i=p.size();i>0;--i)
                    {
                        q[i] = p[i] + lam * xi[i];
                    }
                    return Phi(A,q,b,z,r);
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(spcj);
            };
        }
       
        template <>
        void sp_cgrad<real_t>::solve( array<real_t> &p, const sp_matrix<real_t> &A, const array<real_t> &b )
        {
            assert(p.size()==b.size());
            assert(A.rows==A.cols);
            assert(A.rows==b.size());
            assert(A.cols==p.size());
            
            static const real_t ftol = REAL(1e-5);
            
            const size_t n = p.size();
            g.make(n,0);
            h.make(n,0);
            xi.make(n,0);
            z.make(n,0);
            q.make(n,0);
            r.make(n,0);
            
            spcj                      wrapper(A,b,p,q,xi,z,r);
            numeric<real_t>::function phi( &wrapper, & spcj::phi );
            
            //__________________________________________________________________
            //
            // initialize algorithm
            //__________________________________________________________________
            real_t f0 = Phi(A,p,b,xi,r);
            for(size_t j=n;j>0;--j)
            {
                g[j] = -xi[j];
                xi[j]=h[j]=g[j];
            }
            
            
            size_t count = 0;
            while(true)
            {
                //______________________________________________________________
                //
                // line minimisation of phi along xi
                //______________________________________________________________
                triplet<real_t> xx = { 0,  1,         0 };
                triplet<real_t> ff = { f0, phi(xx.b), 0 };
                bracket<real_t>::expand(phi, xx, ff);
                minimize<real_t>(phi, xx, ff, ftol);
                for(size_t j=n;j>0;--j)
                {
                     p[j] += xx.b * xi[j];
                }
                const real_t f1 = Phi(A,p,b,xi,r); // gradient is recomputed in xi
                
                //______________________________________________________________
                //
                // test convergence
                //______________________________________________________________
                
                //______________________________________________________________
                //
                // conjugated direction
                //______________________________________________________________
                real_t gg  = 0;
                real_t dgg = 0;
                for(size_t j=n;j>0;--j)
                {
                    gg  += g[j] * g[j];
                    dgg += (xi[j]+g[j])*g[j];
                }
                
                if(gg<=0)
                {
                    return; //! special numeric case
                }
                
                const real_t gam = dgg/gg;
                for(size_t j=n;j>0;--j)
                {
                    g[j] = -xi[j];
                    xi[j]=h[j]=g[j]+gam*h[j];
                }
                
                //______________________________________________________________
                //
                // continue
                //______________________________________________________________
                f0 = f1;
                if(++count>10)
                    break;
            }
        }
        
    }
    
}