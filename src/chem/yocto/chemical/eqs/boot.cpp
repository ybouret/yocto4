#include "yocto/chemical/boot.hpp"
#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/crout.hpp"

#include "yocto/exception.hpp"
#include "yocto/code/rand.hpp"

namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        namespace
        {
            //__________________________________________________________________
            //
            //! integer Gram-Schmidt
            //__________________________________________________________________
            static inline
            bool igs( imatrix_t &A)
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
            //! Boot Algorithm Manager
            //__________________________________________________________________
            class BootMgr
            {
            public:
                equilibria        &eqs;
                const size_t       M;
                const size_t       N;
                const size_t       Nc;
                const imatrix_t   &Nu;
                vector_t          &C;
                vector_t          &Gamma;
                matrix_t          &Phi;
                matrix_t          &W;
                vector_t          &xi;
                vector_t          &dC;
                const array<bool> &active;
                ivector_t         &beta;
                vector_t           Cfixed;
                uvector_t          Jfixed;
                const size_t       Mfixed;
                vector_t           U;
                imatrix_t          P;
                imatrix_t          Q;
                vector_t           Lam;
                integer_t          detJ;
                imatrix_t          MU;     //!< MU=P'*adjoint(P*P') TODO: check..
                vector_t           dL;
                
                inline BootMgr(equilibria &cs,
                               const boot &loader) :
                eqs(cs),
                M(cs.M),
                N(cs.N),
                Nc(loader.size()),
                Nu( cs.Nu ),
                C(cs.C),
                Gamma(cs.Gamma),
                Phi(cs.Phi),
                W(cs.W),
                xi(cs.xi),
                dC(cs.dC),
                active(cs.active),
                beta(cs.beta),
                Cfixed(M,0),
                Jfixed(M,0),
                Mfixed(0),
                U(M,0),
                P(Nc,M),
                Q(N,M),
                Lam(Nc,0.0),
                detJ(0),
                MU(M,Nc),
                dL(Nc,0)
                {
                    //__________________________________________________________
                    //
                    // create data from boot loader
                    //__________________________________________________________
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
                    std::cerr << "Lam0=" << Lam << std::endl;
                    std::cerr << "P0="   << P   << std::endl;
                    
                    //__________________________________________________________
                    //
                    // factorize data: get constant and check structure
                    //__________________________________________________________
                    rewrite_constraints();
                    std::cerr << "Lam=" << Lam << std::endl;
                    std::cerr << "P="   << P   << std::endl;
                    
                    
                    //__________________________________________________________
                    //
                    // check constraints rank and compute helper matrix
                    //__________________________________________________________
                    {
                        imatrix_t J(Nc,Nc);
                        tao::mmul_rtrn(J, P, P);
                        std::cerr << "J=" << J << std::endl;
                        detJ = determinant_of(J);
                        std::cerr << "detJ=" << detJ << std::endl;
                        if(!detJ)
                            throw exception("singular set of chemical constraints!");
                        imatrix_t AJ(Nc,Nc);
                        adjoint(AJ, J);
                        std::cerr << "AJ=" << AJ << std::endl;
                        tao::mmul_ltrn(MU, P, AJ);
                    }
                    
                    //__________________________________________________________
                    //
                    // check orthogonal space
                    //__________________________________________________________
                    {
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
                        if(!igs(F))
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
                    std::cerr << "Q=" << Q << std::endl;
                    
                    std::cerr << "MU=" << MU << std::endl;
                    
                    for(size_t j=M;j>0;--j) C[j] =  alea<double>();
                    
                    compute_U();
                    for(size_t j=M;j>0;--j)
                    {
                        C[j] = (detJ*C[j] +U[j])/detJ;
                    }
                    std::cerr << "CU=" << C << std::endl;
                    tao::mul(dL, P, C);
                    std::cerr << "dL=" << dL << std::endl;
                    
                    
                }
                
                inline ~BootMgr() throw() {}
                
                
                inline void compute_U() throw()
                {
                    tao::mul(dL, P, C);
                    tao::subp(dL, Lam);
                    tao::mul(U,MU,dL);
                }
                
                inline void set_Cfixed() throw()
                {
                    for(size_t j=Mfixed;j>0;--j)
                    {
                        C[Jfixed[j]] = Cfixed[j];
                    }
                }
                
                inline  void compute_U_and_update_C() throw()
                {
                    compute_U();
                    for(size_t j=M;j>0;--j)
                    {
                        C[j] = (detJ*C[j]+U[j])/detJ;
                    }
                    set_Cfixed();
                }
                
                
                inline void clear_Phi() throw()
                {
                    for(size_t i=N;i>0;--i)
                    {
                        array<double> &phi = Phi[i];
                        for(size_t j=Mfixed;j>0;--j)
                        {
                            phi[Jfixed[j]] = 0;
                        }
                    }
                }
                
                
                
                void rewrite_constraints();
                
                void solve();
                
                inline bool has_bad() throw()
                {
                    bool bad = false;
                    for(size_t j=M;j>0;--j)
                    {
                        beta[j] = 0;
                        if(active[j]&&C[j]<0)
                        {
                            beta[j] = 1;
                            bad = true;
                        }
                    }
                    return bad;
                }
                
                void rebalance();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(BootMgr);
            };
            
            //__________________________________________________________________
            //
            //
            // Tuning constraints
            //
            //__________________________________________________________________

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
            
            void BootMgr::rewrite_constraints()
            {
                Jfixed.free();
                Cfixed.free();
                
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
                (size_t&)Mfixed = Cfixed.size();
                co_qsort(Jfixed,Cfixed);
                std::cerr << "Jfixed=" << Jfixed << std::endl;
                std::cerr << "Cfixed=" << Cfixed << std::endl;
            }
            
            //__________________________________________________________________
            //
            //
            // Balancing C
            //
            //__________________________________________________________________
            void BootMgr:: rebalance()
            {
                while( has_bad() )
                {
                    std::cerr << "beta=" << beta << std::endl;
                    break;
                }
            }
            
            //__________________________________________________________________
            //
            //
            // solve all
            //
            //__________________________________________________________________
            void BootMgr::solve()
            {
                std::cerr << "--solving " << std::endl;
                std::cerr << "K=" << eqs.K << std::endl;
                
                //-- generate random C
                for(size_t j=M;j>0;--j)
                {
                    C[j] = alea<double>();
                }
                set_Cfixed();
                
                std::cerr << "C=" << C << std::endl;
                
                //-- compute status: Gamma and Phi
                eqs.updateGammaAndPhi();
                std::cerr << "Gamma=" << Gamma << std::endl;
                std::cerr << "Phi0="  << Phi   << std::endl;
                clear_Phi();
                std::cerr << "Phi ="  << Phi   << std::endl;
                
                //-- compute U and update C
                compute_U_and_update_C();
                std::cerr << "Cu=" << C << std::endl;
                
                //-- compute W=Phi*Q'
                tao::mmul_rtrn(W,Phi,Q);
                std::cerr << "W=" << W << std::endl;
                if( !crout<double>::build(W) )
                {
                    // WHAT DO I DO ?
                    std::cerr << "Singular Value..." << std::endl;
                    exit(1);
                }
                
                //-- compute xi = -(Gamma+Phi*U)
                tao::mul(xi, Phi, U);
                for(size_t i=N;i>0;--i)
                {
                    xi[i] = -(detJ*Gamma[i]+xi[i])/detJ;
                }
                crout<double>::solve(W, xi);
                std::cerr << "xi=" << xi << std::endl;
                
                //-- compute dC = Q'*xi and add to C
                tao::mul_trn(dC, Q, xi);
                std::cerr << "dC=" << dC << std::endl;
                tao::add(C,dC);
                set_Cfixed();
                
                std::cerr << "C=" << C << std::endl;
                
                //-- rebalance
                rebalance();
            }
            
        }
        
        void equilibria:: solve( const boot &loader, const double t)
        {
            
            if(N>M)
                throw exception("equilibria: not enough species!");
            
            const size_t Nc = loader.size();
            if(Nc+N!=M)
                throw exception("equilibria: M=%u != N=%u + Nc=%u", unsigned(M), unsigned(N), unsigned(Nc) );
            
            if(Nc<=0)
            {
                //______________________________________________________________
                //
                // Special case
                //______________________________________________________________
                std::cerr << "Not Handled: no constraints" << std::endl;
                exit(1);
                
            }
            else
            {
                BootMgr mgr(*this,loader);
                computeK(t);
                mgr.solve();
                
                
            }
            
        }
        
        
    }
    
}

