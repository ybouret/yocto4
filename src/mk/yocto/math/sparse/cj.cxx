#include "yocto/math/ztype.hpp"
#include "yocto/math/sparse/cj.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/sparse/algebra.hpp"

namespace yocto
{
    
    namespace math
    {
        
        typedef algebra<real_t>    mkl;
        typedef sp_algebra<real_t> skl;
        
        template <>
        sp_cgrad<real_t>:: ~sp_cgrad() throw()
        {
            
        }
        
        template <>
        sp_cgrad<real_t>:: sp_cgrad() throw() :
        g(),h(),xi(),z()
        {
            
        }
        
        template <>
        sp_cgrad<real_t>:: sp_cgrad(size_t n) :
        g(n,as_capacity),
        h(n,as_capacity),
        xi(n,as_capacity),
        z(n,as_capacity),
        q(n,as_capacity)
        {
            
        }
        
        namespace
        {
            static inline
            real_t Phi(const sp_matrix<real_t> &A,
                       const array<real_t>     &p,
                       const array<real_t>     &b,
                       array<real_t>           &gr )
            {
                // gradient
                mkl::neg(gr,b);
                skl::muladd(gr,A,p);
                
                // phi
                real_t ans = REAL(0.0);
                for(size_t i=gr.size();i>0;--i)
                {
                    const real_t xx  = gr[i];
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
                             array<real_t>           &zref) throw() :
                A(Aref),
                b(bref),
                p(pref),
                q(qref),
                xi(xiref),
                z(zref)
                {
                }
                
                const sp_matrix<real_t> &A;
                const array<real_t>     &b;
                const array<real_t>     &p;
                array<real_t>           &q;
                const array<real_t>     &xi;
                array<real_t>           &z;
                
                real_t phi( real_t lam )
                {
                    for(size_t i=p.size();i>0;--i)
                    {
                        q[i] = p[i] + lam * xi[i];
                    }
                    return Phi(A,q,b,z);
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
            
            
            const size_t n = p.size();
            g.make(n,0);
            h.make(n,0);
            xi.make(n,0);
            z.make(n,0);
            q.make(n,0);
            
            spcj                      wrapper(A,b,p,q,xi,z);
            numeric<real_t>::function phi( &wrapper, & spcj::phi );
            
            //__________________________________________________________________
            //
            // initialize algorithm
            //__________________________________________________________________
            real_t fp = Phi(A,p,b,xi);
            for(size_t j=n;j>0;--j)
            {
                g[j] = -xi[j];
                xi[j]=h[j]=g[j];
            }
            while(true)
            {
                //______________________________________________________________
                //
                // line minimisation of phi along xi
                //______________________________________________________________
                
            }
        }
        
    }
    
}