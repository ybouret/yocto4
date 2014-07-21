#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"

#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/sort/quick.hpp"

#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/svd.hpp"


namespace yocto
{
    namespace chemical
    {
        
        typedef math::algebra<double> mkl;
        
        static inline
        bool check_valid( const array<double> &P ) throw()
        {
            for(size_t j=P.size();j>0;--j)
            {
                if(0!=int(P[j]))
                    return true;
            }
            return false;
        }
        
        static inline int __rint( double x ) throw()
        {
            return int( floor(x+0.5) );
        }
        
        static inline
        size_t count_items( size_t &s, const array<double> &P ) throw()
        {
            size_t count = 0;
            s = 0;
            for(size_t j=P.size();j>0;--j)
            {
                if(int(P[j])!=0)
                {
                    s = j;
                    ++count;
                }
            }
            return count;
        }
        
        
#if 0
        static inline void tweak( array<double> &q )
        {
            const size_t M = q.size();
            unsigned alpha=1;
            for(;;++alpha)
            {
                const unsigned alpha2 = alpha*alpha;
                unsigned       sum_f2 = 0;
                for(size_t j=M;j>0;--j)
                {
                    sum_f2 +=  __rint(alpha2*q[j]*q[j]);
                }
                std::cerr << "f2=" << sum_f2 << "/alpha2=" << alpha2 << std::endl;
                if(sum_f2==alpha2)
                    break;
            }
            
            for(size_t j=M;j>0;--j)
            {
                q[j] = __rint(alpha*q[j]);
            }
            
        }
#endif
        
        void boot:: operator()(array<double>    &C,
                               const collection &lib,
                               equilibria       &cs,
                               double            t )
        {
            cs.startup(lib);
            try
            {
                const size_t N = cs.N;
                const size_t M = cs.M;
                assert(C.size()>=M);
                if(N>M)
                    throw exception("too many reactions");
                
                const size_t Nc = constraints.size();
                if(Nc+N!=M)
                    throw exception("#species=%u != #reactions=%u + #constraints=%u ", unsigned(M), unsigned(N), unsigned(Nc));
                
                if(Nc<=0)
                {
                    std::cerr << "NOT IMPLEMENTED : NO CONSTRAINTS" << std::endl;
                    return;
                }
                
                //______________________________________________________________
                //
                // Fill the P matrix and Lambda vector
                //______________________________________________________________
                matrix_t  P(Nc,M);
                vector_t  Lambda(Nc,0.0);
                for(size_t i=1;i<=Nc;++i)
                {
                    const constraint &cc = *constraints[i];
                    Lambda[i] = cc.sum;
                    cc.fill(P[i]);
                }
                
                std::cerr << "P0="      << P      << std::endl;
                std::cerr << "Lambda0=" << Lambda << std::endl;
                
                
                //______________________________________________________________
                //
                // optimize with singleton(s)
                //______________________________________________________________
                matrix_t             &Nu = cs.Nu;
                sorted_vector<size_t> single(Nc,as_capacity);
                
                vector<double>        Cfixed(Nc,as_capacity);
                vector<size_t>        ifixed(Nc,as_capacity);
                bool                  changed = false;
                
                for(size_t i=1;i<=Nc;++i)
                {
                    //__________________________________________________________
                    //
                    // how many not zero items...
                    //__________________________________________________________
                    size_t       s     = 0;
                    const size_t count = count_items(s,P[i]);
                    
                    //__________________________________________________________
                    //
                    // None: bad
                    //__________________________________________________________
                    if(count<=0)
                        throw exception("unexpected empty constraint[%u]!", unsigned(i));
                    
                    
                    //__________________________________________________________
                    //
                    // general case
                    //__________________________________________________________
                    if(count>1)
                        continue;
                    
                    //__________________________________________________________
                    //
                    // a singleton !
                    //__________________________________________________________
                    assert(1==count);
                    if( !single.insert(s) )
                        throw exception("multiple single constraint on species #%u", unsigned(s) );
                    
                    
                    //__________________________________________________________
                    //
                    // simplify the projection matrix
                    //__________________________________________________________
                    changed = true;
                    
                    assert(0!=P[i][s]);
                    const double lhs = (Lambda[i] /= P[i][s]);
                    P[i][s] = 1;
                    Cfixed.push_back(lhs);
                    ifixed.push_back(s);
                    
                    //__________________________________________________________
                    //
                    // simplify P: inform other rows of that case
                    //__________________________________________________________
                    for(size_t k=1;k<=Nc;++k)
                    {
                        if(i!=k)
                        {
                            array<double>    &Pk = P[k];
                            double           &p  = Pk[s];
                            if(p>0)
                            {
                                Lambda[k] -= p*lhs;
                                p          = 0.0;
                                if( !check_valid(Pk) )
                                    throw exception("degenerate constraints");
                            }
                        }
                    }
                    
                    //__________________________________________________________
                    //
                    // simplify Nu from fixed species
                    //__________________________________________________________
                    for(size_t k=N;k>0;--k)
                    {
                        Nu[k][s] = 0;
                    }
                    
                    
                }
                if(changed)
                {
                    std::cerr << "#has changed..." << std::endl;
                    std::cerr << "Nu=" << Nu << std::endl;
                    cs.find_active_species();
                    std::cerr << "P=" << P << std::endl;
                    std::cerr << "Lambda=" << Lambda << std::endl;
                    
                    co_qsort(ifixed, Cfixed);
                    std::cerr << "ifixed=" << ifixed << std::endl;
                    std::cerr << "Cfixed=" << Cfixed << std::endl;
                }
                const size_t Nf = ifixed.size();
                
                //______________________________________________________________
                //
                // Check rank using the Gramian matrix
                //______________________________________________________________
                matrix_t P2(Nc,Nc);
                for(size_t i=Nc;i>0;--i)
                {
                    for(size_t j=i;j>0;--j)
                    {
                        double sum = 0;
                        for(size_t k=M;k>0;--k)
                        {
                            sum += P[i][k] * P[j][k];
                        }
                        P2[i][j] = P2[j][i] = sum;
                    }
                }
                std::cerr << "P2=" << P2 << std::endl;
                matrix_t AP2(P2);
                const int detP2 = __rint(math::__determinant_of(AP2));
                std::cerr << "detP2=" << detP2 << std::endl;
                if(!detP2)
                {
                    throw exception("singular set of constraints!");
                }
                
                //______________________________________________________________
                //
                // Compute the adjoint matrix to avoid precision lost
                //______________________________________________________________
                math::adjoint(AP2,P2);
                for(size_t i=1;i<=Nc;++i)
                {
                    for(size_t j=1;j<=Nc;++j)
                    {
                        AP2[i][j] = __rint(AP2[i][j]);
                    }
                }
                std::cerr << "AP2=" << AP2 << std::endl;
                
                //______________________________________________________________
                //
                // Compute the Cstar concentration
                //______________________________________________________________
                vector_t U(Nc,0);
                mkl::mul(U,AP2,Lambda);
                vector_t Cstar(M,0);
                mkl::mul_trn(Cstar, P, U);
                for(size_t j=M;j>0;--j) Cstar[j] /= detP2;
                std::cerr << "Cstar=" << Cstar << std::endl;
                
                //______________________________________________________________
                //
                // Compute the orthogonal space
                //______________________________________________________________
                matrix_t Q(N,M);
                {
                    matrix_t F(M,M);
                    for(size_t i=1;i<=Nc;++i)
                    {
                        for(size_t j=1;j<=M;++j)
                        {
                            F[j][i] = P[i][j];
                        }
                    }
                    vector_t svd_w(M,0.0);
                    matrix_t svd_V(M,M);
                    if(!math::svd<double>::build(F,svd_w, svd_V))
                    {
                        throw exception("cannot find orthonormal basis");
                    }
                    for(size_t i=1;i<=N;++i)
                    {
                        for(size_t j=1;j<=M;++j)
                        {
                            Q[i][j] = F[j][Nc+i];
                        }
                    }
                }
                std::cerr << "Q=" << Q << std::endl;
                
                
                //______________________________________________________________
                //
                // Compute the scaling from constants etc
                //______________________________________________________________
                cs.computeK(t);
                vector_t Cscal(N,0);
                for(size_t i=1;i<=N;++i)
                {
                    const double K   = cs.K[i];
                    const double dNu = cs.dNu[i];
                    if(dNu!=0)
                    {
                        Cscal[i] = pow(K,1.0/dNu);
                    }
                    else
                    {
                        Cscal[i] = 0;
                    }
                }
                std::cerr << "K="     << cs.K  << std::endl;
                std::cerr << "Cscal=" << Cscal << std::endl;
                
                matrix_t &Phi   = cs.Phi;
                vector_t &Gamma = cs.Gamma;
                matrix_t &W     = cs.W;
                vector_t &dX    = cs.dC;
                vector_t &xi    = cs.xi;
                vector_t  X(M,0);
                
                vector_t dL(Nc,0);
                
                for(size_t iter=0;iter<1;++iter)
                {
                    // create a trial point
                    for(size_t j=M;j>0;--j)
                    {
                        X[j] = 0;
                    }
                    for(size_t j=Nf;j>0;--j)
                    {
                        X[ ifixed[j] ] = Cfixed[j];
                    }
                    for(size_t i=N;i>0;--i)
                    {
                        const double cc = Cscal[i];
                        for(size_t j=M;j>0;--j)
                        {
                            if(cs.active[j]>0)
                            {
                                X[j] += ran() * cc;
                            }
                        }
                    }
                    
                    for(size_t sub=0;sub<2;++sub)
                    {
                        if( !cs.normalize(t, X))
                        {
                            throw exception("Beuh");
                        }
                        
                        std::cerr << "X=" << X << std::endl;
                        mkl::set(dL,Lambda);
                        mkl::mulsub(dL, P, X);
                        std::cerr << "dL=" << dL << std::endl;
                        mkl::mul(U,AP2,dL);
                        mkl::mul_trn(dX, P, U);
                        std::cerr << "dX=" << dX << std::endl;
                        
                        cs.updateGammaAndPhi(X);
                        mkl::mul_rtrn(W, Phi, Nu);
                        if(!cs.LU.build(W))
                        {
                            std::cerr << "invalid composition" << std::endl;
                            return;
                        }
                        mkl::neg(cs.xi, Gamma);
                        mkl::mulsub(xi, Phi, dX);
                        lu_t::solve(W, xi);
                        std::cerr << "xi=" << xi << std::endl;
                        mkl::muladd_trn(dX, Nu, xi);
                        std::cerr << "dX1=" << dX << std::endl;
                        mkl::add(X,dX);
                        std::cerr << "X1=" << X << std::endl;
                    }
                }
                
                
                cs.restore_topology();
                
            }
            catch(...)
            {
                cs.restore_topology();
                throw;
            }
        }
    }
}
