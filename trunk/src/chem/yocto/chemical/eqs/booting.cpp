#include "yocto/chemical/equilibria.hpp"
#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/sort/quick.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/crout.hpp"


namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        namespace
        {
            //------------------------------------------------------------------
            //
            // rewriting constraints
            //
            //------------------------------------------------------------------
            static inline
            size_t count_species( const array<ptrdiff_t> &row, size_t &single ) throw()
            {
                size_t ans = 0;
                for(size_t j=row.size();j>0;--j)
                {
                    if(0!=row[j])
                    {
                        single = j;
                        ++ans;
                    }
                }
                return ans;
            }
            
            static inline
            void rewrite_constraints(imatrix_t       &P,
                                     vector_t        &Lam,
                                     uvector_t       &Jfixed,
                                     vector_t        &Cfixed,
                                     const bvector_t &active
                                     )
            
            {
                
                assert(0==Jfixed.size());
                assert(0==Cfixed.size());
                const size_t Nc = P.rows;
                size_t ifixed = 0; // last fixed index
                for(size_t i=1;i<=Nc;++i)
                {
                    size_t       jfixed = 0;
                    const size_t ns = count_species(P[i],jfixed);
                    switch(ns)
                    {
                        case 0:
                            throw exception("boot: deduced an empty constraint");
                            
                        case 1:
                        {
                            std::cerr << "Fixed species #" << jfixed << std::endl;
                            //-- re order matrix
                            ++ifixed;
                            P.swap_rows(ifixed,i);
                            cswap(Lam[ifixed],Lam[i]);
                            
                            //-- simplify row/level
                            const double cfixed = (Lam[ifixed]/=P[ifixed][jfixed]);
                            P[ifixed][jfixed] = 1;
                            
                            //-- check possible
                            if( (cfixed<0) && active[jfixed])
                                throw exception("Fixed species #%u set to a negative value", unsigned(jfixed));
                            
                            //-- propagate information below
                            for(size_t k=ifixed+1;k<=Nc;++k)
                            {
                                const ptrdiff_t p = P[k][jfixed];
                                Lam[k] -= p * cfixed;
                                P[k][jfixed] = 0;
                            }
                            
                            //-- register who/value
                            Jfixed.push_back(jfixed);
                            Cfixed.push_back(cfixed);
                            
                            
                        } break;
                            
                        default:
                            break;
                    }
                }
                co_qsort(Jfixed,Cfixed);
                std::cerr << "Jfixed=" << Jfixed << std::endl;
                std::cerr << "Cfixed=" << Cfixed << std::endl;
            }
            
            
            //------------------------------------------------------------------
            //
            // orthogonal space to constraints
            //
            //------------------------------------------------------------------
            
            //__________________________________________________________________
            //
            // integer Gram-Schmidt
            //__________________________________________________________________
            static inline
            bool IGS( imatrix_t &A)
            {
                const size_t dim = A.cols;
                const size_t num = A.rows;
                ivector_t    usq(num,0);
                ivector_t    w(dim,0);
                
                for(size_t i=1;i<=num;++i)
                {
                    array<integer_t> &v  = A[i];
                    for(size_t k=1;k<i;++k)
                    {
                        const array<integer_t> &u  = A[k];
                        const integer_t         u2 = usq[k];
                        const integer_t         uv = tao::dot(u,v);
                        for(size_t j=dim;j>0;--j)
                        {
                            w[j] = u2* v[j] - uv * u[j];
                        }
                        tao::simplify(w);
                        for(size_t j=dim;j>0;--j)
                        {
                            v[j] = w[j];
                        }
                    }
                    const integer_t last = usq[i] = tao::norm_sq(v);
                    if(!last)
                        return false;
                }
                return true;
            }
            
            //__________________________________________________________________
            //
            // Compute the Q-space
            //__________________________________________________________________
            static inline
            void compute_Q( imatrix_t &Q, const imatrix_t &P, const imatrix_t &Nu)
            {
                const size_t M  = P.cols;
                const size_t Nc = P.rows;
                const size_t N  = Q.rows;
                imatrix_t F(M,M);
                
                for(size_t i=1;i<=Nc;++i)
                {
                    for(size_t j=M;j>0;--j)
                    {
                        F[i][j] = P[i][j];
                    }
                }
                
                for(size_t i=1;i<=N;++i)
                {
                    for(size_t j=M;j>0;--j)
                    {
                        F[i+Nc][j] = Nu[i][j];
                    }
                }
                if(!IGS(F))
                {
                    throw exception("singular set of chemical constraints+topology");
                }
                for(size_t i=1;i<=N;++i)
                {
                    for(size_t j=M;j>0;--j)
                    {
                        Q[i][j] = F[i+Nc][j];
                    }
                }
            }
            
            
            static inline
            ptrdiff_t compute_Xstar(vector_t        &Xstar,
                                    const imatrix_t &P,
                                    const vector_t  &Lam)
            {
                const size_t Nc = P.rows;
                imatrix_t    J(Nc,Nc);
                tao::mmul_rtrn(J, P, P);
                const ptrdiff_t detJ = determinant_of(J);
                if(!detJ)
                {
                    throw exception("singular constraints");
                }
                imatrix_t AJ(Nc,Nc);
                adjoint(AJ, J);
                vector_t AJL(Nc,0);
                tao::mul(AJL,AJ,Lam);
                tao::mul_trn(Xstar,P,AJL);
                
                return detJ;
            }
            
            
            static inline
            void compute_C(vector_t        &C,
                           const vector_t  &Xstar,
                           const imatrix_t &Q,
                           const vector_t  &xi,
                           const ptrdiff_t  detJ,
                           const uvector_t &Jfixed,
                           const vector_t  &Cfixed
                           )
            {
                tao::mul_trn(C, Q, xi);
                for(size_t j=C.size();j>0;--j)
                {
                    C[j] = (C[j]+Xstar[j])/detJ;
                }
                
                for(size_t j=Jfixed.size();j>0;--j)
                {
                    C[ Jfixed[j] ] = Cfixed[j];
                }
            }
            
            
        }
        
        
        
        
        void equilibria:: load(const boot &loader, const double t)
        {
            
            //__________________________________________________________________
            //
            //
            // Sanity check
            //
            //__________________________________________________________________
            
            if(N>M)
                throw exception("equilibria: not enough species!");
            
            const size_t Nc = loader.size();
            if(Nc+N!=M)
                throw exception("equilibria: M=%u != N=%u + Nc=%u", unsigned(M), unsigned(N), unsigned(Nc) );
            
            //__________________________________________________________________
            //
            //
            // Special cases
            //
            //__________________________________________________________________
            if(Nc<=0)
            {
                //______________________________________________________________
                //
                // Special case
                //______________________________________________________________
                std::cerr << "Not Handled: no constraints" << std::endl;
                exit(1);
                
            }
            
            
            //__________________________________________________________________
            //
            //
            // Generic case
            //
            //__________________________________________________________________
            imatrix_t P(Nc,M);
            vector_t  Lam(Nc,0);
            
            //__________________________________________________________________
            //
            // create data from boot loader
            //__________________________________________________________________
            for(size_t i=Nc;i>0;--i)
            {
                const constraint &c = *loader[i];
                Lam[i] = c.level;
                for(size_t j=c.count();j>0;--j)
                {
                    const constraint::member &m = c[j];
                    P[i][m.sp->indx] = m.weight;
                }
            }
            
            //__________________________________________________________________
            //
            // factorize data: get constant and check structure
            //__________________________________________________________________
            uvector_t Jfixed(M,as_capacity);
            vector_t  Cfixed(M,as_capacity);
            rewrite_constraints(P,Lam,Jfixed,Cfixed,active);
            std::cerr << "Lam=" << Lam << std::endl;
            std::cerr << "P="   << P   << std::endl;
            bvector_t local_active(M,false);
            tao::set(local_active,active);
            for(size_t j=Jfixed.size();j>0;--j)
            {
                local_active[ Jfixed[j] ] = false;
            }
            std::cerr << "active=" << active << std::endl;
            std::cerr << "localA=" << local_active << std::endl;
            
            vector_t Xstar(M,0);
            const ptrdiff_t detJ = compute_Xstar( Xstar, P, Lam);
            std::cerr << "Xstar=" << Xstar << std::endl;
            std::cerr << "detJ =" << detJ  << std::endl;
            
            //__________________________________________________________________
            //
            // compute orthogonal space
            //__________________________________________________________________
            imatrix_t Q(N,M);
            compute_Q(Q,P,Nu);
            std::cerr << "Q=" << Q << std::endl;
            
            
            
            //__________________________________________________________________
            //
            //
            // Algorithm
            //
            //__________________________________________________________________
            computeK(t);
            
            for(size_t i=N;i>0;--i)
            {
                xi[i] = ran();
                xi[i] = 0;
            }
            compute_C(C,Xstar,Q,xi,detJ,Jfixed,Cfixed);
            std::cerr << "C0=" << C << std::endl;
            
            rebalance_with(Q,local_active);
            
        }
        
        
        bool equilibria:: rebalance_with(const imatrix_t &Q, const bvector_t &local_active)
        {
            
            std::cerr << "\tCb0=" << C << std::endl;
            size_t count = 0;
            while(true)
            {
                ++count;
                if(count>M)
                {
                    std::cerr << "-- too many steps to rebalance" << std::endl;
                    return false;
                }
                //______________________________________________________________
                //
                // detect invalid concentrations
                //______________________________________________________________
                bool has_bad = false;
                for(size_t j=M;j>0;--j)
                {
                    beta[j] = 0;
                    if(local_active[j]&&C[j]<0)
                    {
                        beta[j] = 1;
                        has_bad = true;
                    }
                    
                }
                
                if(!has_bad)
                {
                    std::cerr << "CB=" << C << std::endl;
                    return true;
                }
                
                //______________________________________________________________
                //
                // compute integer step
                //______________________________________________________________
                tao::mul(xip,Q,beta);
                tao::mul_trn(dCp, Q, xip);
                
                std::cerr << "beta=" << beta << std::endl;
                std::cerr << "dCp =" << dCp  << std::endl;
                
                //______________________________________________________________
                //
                // find max step
                //______________________________________________________________
                alpha.free();
                aindx.free();
                for(size_t j=M;j>0;--j)
                {
                    if(local_active[j])
                    {
                        const double    Cj = C[j];
                        const ptrdiff_t Dj = dCp[j];
                        if(Dj<0)
                        {
                            if(Cj<=0)
                            {
                                std::cerr << "-- negative step for a negative concentration" << std::endl;
                                return false;
                            }
                            else
                            {
                                alpha.push_back(Cj/(-Dj));
                                aindx.push_back(j);
                            }
                        }
                        else
                        {
                            
                            if(Dj>0)
                            {
                                if(Cj<0)
                                {
                                    alpha.push_back((-Cj)/Dj); // minimal step
                                    aindx.push_back(j);
                                }
                            }
                            else
                            {
                                // do nothing
                            }
                        }
                    }
                    
                }
                
                if(alpha.size()<=0)
                {
                    std::cerr << "-- no d.o.f. to rebalance" << std::endl;
                    return false;
                }
                
                co_qsort(alpha, aindx);
                const double factor = alpha[1];
                
                if(factor<=0)
                {
                    std::cerr << "-- blocked rebalance" << std::endl;
                    return false;
                }
                
                
                //______________________________________________________________
                //
                // carefull addition
                //______________________________________________________________
                for(size_t j=M;j>0;--j)
                {
                    if(local_active[j])
                    {
                        const ptrdiff_t Dj = dCp[j];
                        const double    Cj = C[j];
                        C[j] += factor * Dj;
                        if(Dj>0)
                        {
                            if(Cj<0&&C[j]>=0)
                            {
                                C[j] = 0;
                            }
                        }
                        else
                        {
                            if(Dj<0)
                            {
                                if(Cj>=0&&C[j]<=0)
                                {
                                    C[j] = 0;
                                }
                            }
                        }
                    }
                }
                C[aindx[1]] = 0;
            }
            
        }
        
        
    }
    
}
