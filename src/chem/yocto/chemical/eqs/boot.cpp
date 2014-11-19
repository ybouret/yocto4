#include "yocto/chemical/boot.hpp"
#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"

#include "yocto/exception.hpp"
#include "yocto/code/rand.hpp"

namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        namespace
        {
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
            
            class BootMgr
            {
            public:
                const size_t     M;
                const size_t     N;
                const size_t     Nc;
                const imatrix_t &Nu;
                vector_t        &C;
                vector_t         U;
                imatrix_t        P;
                imatrix_t        Q;
                vector_t         Lam;
                integer_t        detJ;
                imatrix_t        MU;     //!< MU=P'*adjoint(P*P') TODO: check..
                vector_t         dL;
                inline BootMgr(equilibria &cs,
                               const boot &loader) :
                M(cs.M),
                N(cs.N),
                Nc(loader.size()),
                Nu( cs.Nu ),
                C(cs.C),
                U(M,0),
                P(Nc,M),
                Q(N,M),
                Lam(Nc,0.0),
                detJ(0),
                MU(M,Nc),
                dL(Nc,0)
                {
                    //______________________________________________________________
                    //
                    // create data from
                    //______________________________________________________________
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
                
                
                inline void compute_U()
                {
                    tao::mul(dL, P, C);
                    tao::subp(dL, Lam);
                    tao::mul(U,MU,dL);
                    std::cerr << "C=" << C << std::endl;
                    std::cerr << "U=" << U << "/" << detJ << std::endl;
                    
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(BootMgr);
            };
            
            
        }
        
        void equilibria:: solve( const boot &loader )
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
                
                
                
            }
            
        }
        
        
    }
    
}

