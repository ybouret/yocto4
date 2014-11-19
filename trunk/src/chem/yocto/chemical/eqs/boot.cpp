#include "yocto/chemical/boot.hpp"
#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        namespace
        {
            static inline
            void igs( imatrix_t &A)
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
                        const integer_t      u2 = usq[k];
                        const integer_t      uv = tao::dot(u,v);
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
                    usq[i] = tao::norm_sq(v);
                }
            }
        }
        
        void equilibria:: solve( const boot &loader )
        {
            
            if(N>=M)
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
                //______________________________________________________________
                //
                // allocate memory
                //______________________________________________________________
                
                imatrix_t P(Nc,M);
                imatrix_t Q(N,M);
                vector_t  Lam(Nc,0);
                
                //______________________________________________________________
                //
                // create data
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
                
                
                imatrix_t P2(Nc,Nc);
                tao::mmul_rtrn(P2, P, P);
                const integer_t detP = determinant_of(P2);
                std::cerr << "detP=" << detP << std::endl;
                if(!detP)
                    throw exception("singular set of chemical constraints!");
                
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
                    //std::cerr << "F=" << F << std::endl;
                    const integer_t detF = determinant_of(F);
                    //std::cerr << "detF=" << detF << std::endl;
                    if(!detF)
                        throw exception("singular set of constraints+topology!");
                    igs(F);
                    //std::cerr << "IGS=" << F << std::endl;
                    for(size_t i=1;i<=N;++i)
                    {
                        for(size_t j=M;j>0;--j)
                        {
                            Q[i][j] = F[i+Nc][j];
                        }
                    }
                }
                std::cerr << "Q=" << Q << std::endl;
                
                
            }
            
        }
        
        
    }
    
}

