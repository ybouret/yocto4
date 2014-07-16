#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/jacobi.hpp"
#include "yocto/math/kernel/det.hpp"

#include "yocto/sort/index.hpp"
#include "yocto/sort/quick.hpp"

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::algebra<double> mkl;
        
        
        //______________________________________________________________________
        //
        // find xi limits
        //______________________________________________________________________
        void equilibria:: find_limits_of(const array<double> &C) throw()
        {
            assert(C.size()>=M);
            
            //__________________________________________________________________
            //
            // Loop on all reactions
            //__________________________________________________________________
            for(size_t i=N;i>0;--i)
            {
                extent              &ex = limits[i];
                const array<double> &nu = Nu[i];
                
                ex.has_forward_limit = ex.has_reverse_limit = false;
                ex.max_forward_value = ex.max_reverse_value = 0.0;
                ex.blocked = false;
                
                //______________________________________________________________
                //
                // Loop on all coefficients
                //______________________________________________________________
                for(size_t j=M;j>0;--j)
                {
                    const int coef = int(nu[j]);
                    
                    if(coef>0)
                    {
                        // we have a product formation
                        const double Xi = max_of<double>(C[j],0.0)/coef;
                        if(ex.has_reverse_limit)
                        {
                            ex.max_reverse_value = min_of<double>(ex.max_reverse_value,Xi);
                        }
                        else
                        {
                            ex.has_reverse_limit = true;
                            ex.max_reverse_value = Xi;
                        }
                    }
                    
                    if(coef<0)
                    {
                        // we have a reactant consumption
                        const double Xi = max_of<double>(C[j],0.0)/(-coef);
                        if(ex.has_forward_limit)
                        {
                            ex.max_forward_value = min_of<double>(ex.max_forward_value,Xi);
                        }
                        else
                        {
                            ex.has_forward_limit = true;
                            ex.max_forward_value = Xi;
                        }
                    }
                }
                
                //______________________________________________________________
                //
                // Check status
                //______________________________________________________________
                if(ex.has_forward_limit      &&
                   ex.has_reverse_limit      &&
                   (ex.max_forward_value<=0) &&
                   (ex.max_reverse_value<=0) )
                {
                    ex.blocked = true;
                }
                
            }
            
        }
        
        void  equilibria:: show_limits() const
        {
            std::cerr << "-------- Limits --------" << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                const extent &ex = limits[i];
                std::cerr << "\tEq" << i << ":";
                if(ex.blocked)
                    std::cerr << " BLOCKED";
                else
                {
                    if(ex.has_forward_limit)
                    {
                        std::cerr << "  max_forward=" << ex.max_forward_value;
                    }
                    if(ex.has_reverse_limit)
                    {
                        std::cerr << "  max_reverse=" << ex.max_reverse_value;
                    }
                }
                std::cerr << std::endl;
            }
            
        }
        
        void  equilibria:: clip_extents(const ptrdiff_t scaling) throw()
        {
            std::cerr << "xi_init=" << xi << "/" << scaling << ";" << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                const extent &ex = limits[i];
                double       &Xi = xi[i];
                std::cerr << "\tEq" << i << ": ";
                
                if(ex.has_forward_limit)
                {
                    std::cerr << "\tmax_fwd=" << ex.max_forward_value;
                    const double top = ex.max_forward_value*scaling;
                    if(Xi>0 && Xi>top)
                    {
                        Xi=top;
                    }
                }
                
                
                if(ex.has_reverse_limit)
                {
                    std::cerr << "\tmax_rev=" << ex.max_reverse_value;
                    const double top = ex.max_reverse_value*scaling;
                    if(Xi<0 && (-Xi)>top)
                    {
                        Xi = -top;
                    }
                }
                
                if( ex.blocked )
                {
                    std::cerr << " | BLOCKED";
                }
                std::cerr << std::endl;
            }
            std::cerr << "xi_clip=" << xi << "/" << scaling << ";" << std::endl;
        }
        
#if 0
        static int compare_abs(const double lhs, const double rhs) throw()
        {
            const double L = fabs(lhs);
            const double R = fabs(rhs);
            return ((L<R) ? -1 : ( (R<L) ? 1 : 0));
        }
        double round_score( int alpha, const array<double> &F )
        {
            double sum = 0;
            for(size_t i=F.size();i>0;--i)
            {
                const double f = fabs(F[i]);
                const double g = __rint(alpha*f);
                std::cerr << "f=" << f << " -> g=" << g << std::endl;
                const double d = fabs(alpha*f-g);
                sum += d;
            }
            return 0;
        }
#endif
        
        
        static inline double __rint(double x) throw() { return floor(x+0.5); }

        static inline bool accept_coef(const int alpha, const array<double> &F) throw()
        {
            assert(alpha>0);
            //std::cerr << "\talpha=" << alpha << " => errmax=" << errmax << std::endl;
            for(size_t i=F.size();i>0;--i)
            {
                const double f     = fabs(F[i]);
                const double af    = alpha*f;
                const double g     = __rint(af);
                const int    r     = int(__rint(fabs(alpha * (af - g))));
                //std::cerr << "\t\tf=" << f << " -> g=" << g << "-> r=" << r << std::endl;
                if(r>0)
                    return false;
            }
            return true;
        }
        
        
        
        bool  equilibria:: validate_old( array<double> &C )
        {
            
            assert(C.size()>=M);
            
            find_limits_of(C);
            show_limits();
            matrix_t     U(M,M);
            mkl::mul_ltrn(U, Nu, Nu);
            matrix_t     V(M,M);
            vector_t     d(M,0);
            std::cerr << "U=" << U << std::endl;
            
            
            if(!math::jacobi<double>::build(U,d, V))
            {
                std::cerr << "Cannot jacobi" << std::endl;
                return false;
            }
            
            // kill zero space
            math::jacobi<double>::eigsrt(d, V);
            std::cerr << "d0=" << d << std::endl;
            for(size_t i=N+1;i<=M;++i)
            {
                d[i] = 0;
            }
            std::cerr << "d1=" << d << std::endl;
            
            for(size_t i=1;i<=N;++i)
            {
                if(d[i]<=0) throw exception("invalid Nu");
                d[i] = 1.0 / d[i];
            }
            
            std::cerr << "id=" << d << std::endl;
            std::cerr << "V=" << V << std::endl;
            matrix_t J(M,M);
            vector_t F((M*(M-1))/2,as_capacity);

            for(size_t i=M;i>0;--i)
            {
                for(size_t j=M;j>=i;--j)
                {
                    double sum = 0;
                    for(size_t k=N;k>0;--k)
                    {
                        sum += V[i][k] * d[k] * V[j][k];
                    }
                    J[i][j] = J[j][i] = sum;
                    F.push_back(fabs(sum));
                }
            }
            std::cerr << "J0=" << J << std::endl;

            quicksort(F);
            //F.reverse();
            std::cerr << "F=" << F << ";" << std::endl;
            
#if 1
            int alpha = 1;
            while( alpha <= 40 && !accept_coef(++alpha, F) )
                ;
            std::cerr << "alpha0=" << alpha << ";" << std::endl;
            
            //-- regularize J
            for(size_t i=M;i>0;--i)
            {
                for(size_t j=M;j>0;--j)
                {
                    J[i][j] = __rint(alpha*J[i][j]);
                }
            }
            std::cerr << "J1=" << J << std::endl;
#endif
            
            
            
            // build the target vector
            for(size_t j=M;j>0;--j)
            {
                if(active[j]>0 && (C[j]<0))
                {
                    dC[j] = -C[j];
                }
                else
                {
                    dC[j] = 0;
                }
            }
            
            std::cerr << "dC=" << dC << std::endl;
            
            //build lambda * alpha
            
            vector_t lambda(M,0);
            mkl::mul(lambda, J, dC);
            std::cerr << "lam0=" << lambda << std::endl;
            
            for(size_t j=M;j>0;--j)
            {
                //if( active[j]<=0 || (C[j]>=0) ) lambda[j] = 0;
            }
            std::cerr << "lam1=" << lambda << std::endl;
            
            // build xi*alpha
            mkl::mul(xi,Nu,lambda);
            std::cerr << "xi0=" << xi << std::endl;
            
            clip_extents(alpha);
            
            // build dC*alpha
            mkl::mul_trn(dC, Nu, xi);
            
            std::cerr << "delta=" << dC << "/" << alpha << std::endl;
            return false;
            
            
            return true;
        }
        
        bool  equilibria:: validate( array<double> &C )
        {
            
            assert(C.size()>=M);
            
            {
                //-- find bad species
                bad.free();
                for(size_t j=1;j<=M;++j)
                {
                    if( (active[j]>0) && (C[j]<0) )
                    {
                        bad.push_back(j);
                        dC[j] = -C[j];
                    }
                }
                const size_t Mb = bad.size();
                if(Mb<=0)
                    return true;
                std::cerr << "bad=" << bad << std::endl;
                
                //-- find online reactions from limits
                find_limits_of(C);
                show_limits();
                
                online.free();
                for(size_t i=1;i<=N;++i)
                {
                    if( !limits[i].blocked )
                        online.push_back(i);
                }
                const size_t Nq = online.size();
                if(Nq<=0)
                {
                    return false;
                }
                std::cerr << "online=" << online << std::endl;
                // build sub topology
                matrix_t nu(Nq,M);
                for(size_t i=1;i<=Nq;++i)
                {
                    const size_t k=online[i];
                    for(size_t j=M;j>0;--j)
                    {
                        nu[i][j] = Nu[k][j];
                    }
                }
                std::cerr << "nu=" << nu << std::endl;
                
                // build expansion matrix
                matrix_t beta(Mb,Nq);
                for(size_t j=1;j<=Mb;++j)
                {
                    const size_t jj = bad[j];
                    for(size_t i=1;i<=Nq;++i)
                    {
                        const size_t ii = online[i];
                        beta[j][i] = Nu[ii][jj];
                    }
                }
                
                std::cerr << "beta=" << beta << std::endl;
                matrix_t U(Mb,Mb);
                mkl::mul_rtrn(U, beta, beta);
                std::cerr << "U=" << U <<std::endl;
                ptrdiff_t detU = int(math::determinant_of(U));
                std::cerr << "detU=" << detU << std::endl;
                if( !detU )
                {
                    throw exception("Singular subsystem, unexpected");
                }
                
                matrix_t V(Mb,Mb);
                math::adjoint(V, U);
                std::cerr << "V=" << V << std::endl;
                vector_t deltaC(Mb,0);
                for(size_t j=Mb;j>0;--j)
                {
                    deltaC[j] = -C[bad[j]];
                }
                std::cerr << "deltaC=" << deltaC << std::endl;
                vector_t lambda(Mb,0);
                
                // lambda*detU
                mkl::mul(lambda,V,deltaC);
                std::cerr << "lambda=" << lambda << "/" << detU << std::endl;
                
                // xi_prime * detU
                vector_t xi_prime(Nq,0);
                mkl::mul_trn(xi_prime, beta, lambda);
                std::cerr << "xi_prime=" << xi_prime << "/" << detU << std::endl;
                
                // build Xi*detU
                mkl::set(xi,0.0);
                for(size_t i=Nq;i>0;--i)
                {
                    xi[ online[i] ] = xi_prime[i];
                }
                if(detU<0)
                {
                    mkl::neg(xi, xi);
                    detU = -detU;
                    std::cerr << "xi="<< xi << "/" << detU << std::endl;
                }
                clip_extents(detU);
                
                // build dC * detU
                mkl::mul_trn(dC, Nu, xi);
                std::cerr << "dC=" << dC << std::endl;
                
                // add carefully
                for(size_t j=M;j>0;--j)
                {
                    if(active[j]>0)
                    {
                        if(C[j]<0)
                        {
                            
                        }
                        else
                        {
                            
                        }
                    }
                }
            }
            
            
            
            
            return false;
        }
        
        
    }
}
