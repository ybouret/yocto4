#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"

#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/sort/quick.hpp"

#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/svd.hpp"

#include "yocto/code/utils.hpp"


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
        
        static inline integer_t __inorm_sq( const array<double> &v ) throw()
        {
            integer_t ans = 0;
            for(size_t j=v.size();j>0;--j)
            {
                const integer_t ii = __rint(v[j]);
                ans += ii*ii;
            }
            return ans;
        }
        
        static inline integer_t __idot( const array<double> &v, const array<double> &u) throw()
        {
            integer_t ans = 0;
            assert(u.size()==v.size());
            for(size_t j=v.size();j>0;--j)
            {
                ans += __rint(v[j]) * __rint(u[j]);
            }
            return ans;
        }
        
        static inline void __simplify( array<integer_t> &w ) throw()
        {
            integer_t g = 0;
            const size_t n = w.size();
            for(size_t i=n;i>0;--i)
            {
                const integer_t wi = w[i];
                const integer_t ai = wi < 0 ? -wi : wi;
                if(!ai)
                    continue;

                for(size_t j=i;j>0;--j)
                {
                    const integer_t wj  = w[j];
                    const integer_t aj  = wj < 0 ? -wj : wj;
                    if(!aj)
                        continue;
                    
                    const integer_t gij = gcd_of(ai,aj);
                    if(g<=0)
                    {
                        g = gij;
                    }
                    else
                    {
                        g = min_of(g,gij);
                    }
                }
            }
            if(g>0)
            {
                for(size_t i=n;i>0;--i)
                {
                    w[i] /= g;
                }
            }
        }
        
        void boot:: igs( matrix_t &A )
        {
            const size_t dim = A.cols;
            const size_t num = A.rows;
            ivector_t    usq(num,0);
            ivector_t    w(dim,0);
            
            for(size_t i=1;i<=num;++i)
            {
                array<double> &v  = A[i];
                for(size_t k=1;k<i;++k)
                {
                    const array<double> &u  = A[k];
                    const integer_t      u2 = usq[k];
                    const integer_t      uv = __idot(u,v);
                    // construct integer vector
                    for(size_t j=dim;j>0;--j)
                    {
                        w[j] = u2*__rint(v[j]) - uv * __rint(u[j]);
                    }
                    __simplify(w);
                    for(size_t j=dim;j>0;--j)
                    {
                        v[j] = w[j];
                    }
                }
                usq[i] = __inorm_sq(v);
            }
            
        }
        
        
        
        void equilibria::initialize_with(const boot       &loader,
                                         const collection &lib,
                                         const double      t,
                                         array<double>     &C0)
        {
            startup(lib);
            assert(C0.size()>=M);
            try
            {
                
                if(N>M)
                    throw exception("too many reactions");
                
                const size_t Nc = loader.size();
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
                    const boot::constraint &cc = loader[i];
                    Lambda[i] = cc.sum;
                    cc.fill(P[i]);
                }
                
                std::cerr << "P0="      << P      << std::endl;
                std::cerr << "Lambda0=" << Lambda << std::endl;
                
                //______________________________________________________________
                //
                // optimize with singleton(s)
                //______________________________________________________________
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
                    std::cerr << "Nu0=" << Nu0 << std::endl;
                    std::cerr << "Nu="  << Nu << std::endl;
                    find_active_species();
                    std::cerr << "P=" << P << std::endl;
                    std::cerr << "Lambda=" << Lambda << std::endl;
                    
                    co_qsort(ifixed, Cfixed);
                    std::cerr << "ifixed=" << ifixed << std::endl;
                    std::cerr << "Cfixed=" << Cfixed << std::endl;
                }
                const size_t Nf = ifixed.size();
                std::cerr << "#fixed=" << Nf << std::endl;
                
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
                
                matrix_t F(M,M);
                for(size_t i=Nc;i>0;--i)
                {
                    for(size_t j=M;j>0;--j)
                    {
                        F[i][j] = P[i][j];
                    }
                }
                for(size_t i=N;i>0;--i)
                {
                    for(size_t j=M;j>0;--j)
                    {
                        F[Nc+i][j] = Nu[i][j];
                    }
                }
                std::cerr << "F0=" << F << std::endl;
                boot::igs(F);
                std::cerr << "F=" << F << std::endl;
                exit(1);
                
                //______________________________________________________________
                //
                // Compute Xstar
                //______________________________________________________________
                vector_t Xstar(M,0.0);
                vector_t U(Nc,0.0);
                mkl::mul(U,AP2,Lambda);
                mkl::mul_trn(Xstar, P, U);
                for(size_t j=M;j>0;--j) Xstar[j]/=detP2;
                std::cerr << "Xstar=" << Xstar << std::endl;
                
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
                
                
                computeK(t);
                compute_scaled_concentrations();
                std::cerr << "K=" << K << std::endl;
                std::cerr << "scaled=" << scaled << std::endl;
                
                
                //______________________________________________________________
                //
                // Let us start
                //______________________________________________________________
                vector_t X(M,0.0);
                vector_t dL(Nc,0.0);
                vector_t dU(Nc,0);
                vector_t dX(M,0);
                for(size_t j=Nf;j>0;--j)
                {
                    X[ ifixed[j] ] = Cfixed[j];
                }
                if(!compute_trial(X,loader.ran))
                {
                    
                }
                
                
                
                
                //______________________________________________________________
                //
                // All Done ! Restore Topology
                //______________________________________________________________
                restore_topology();
                
            }
            catch(...)
            {
                restore_topology();
                throw;
            }
            
        }
        
        
    }
}
