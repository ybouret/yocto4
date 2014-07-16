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
        
        void  equilibria:: clip_extents() throw()
        {
            std::cerr << "xi_init=" << xi << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                const extent &ex = limits[i];
                double       &Xi = xi[i];
                std::cerr << "\tEq" << i << ": ";
                
                if(ex.has_forward_limit)
                {
                    std::cerr << "\tmax_fwd=" << ex.max_forward_value;
                    if(Xi>0 && Xi>ex.max_forward_value)
                    {
                        Xi=ex.max_forward_value;
                    }
                }
                
                
                if(ex.has_reverse_limit)
                {
                    std::cerr << "\tmax_rev=" << ex.max_reverse_value;
                    if(Xi<0 && (-Xi)>ex.max_reverse_value)
                    {
                        Xi = -ex.max_reverse_value;
                    }
                }
                
                if( ex.blocked )
                {
                    std::cerr << " | BLOCKED";
                }
                std::cerr << std::endl;
            }
            std::cerr << "xi_clip=" << xi << std::endl;
        }
        
#if 1
        static int compare_abs(const double lhs, const double rhs) throw()
        {
            const double L = fabs(lhs);
            const double R = fabs(rhs);
            return ((L<R) ? -1 : ( (R<L) ? 1 : 0));
        }
        
        static inline double __rint(double x) { return floor(x+0.5); }
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
        
        static inline bool accept_coef(const int alpha, const array<double> &F)
        {
            assert(alpha>0);
            double errmax = 0;
            std::cerr << "\talpha=" << alpha << std::endl;
            for(size_t i=F.size();i>0;--i)
            {
                const double f    = fabs(F[i]);
                const double g     = __rint(alpha*f);
                const double err   = fabs(f-g/alpha);
                std::cerr << "\t\tf=" << f << " -> g=" << g << "-> err=" << err << std::endl;
                if(err>errmax) errmax = err;
            }
            std::cerr << "errmax=" << errmax << std::endl;
            return false;
        }
#endif
        
        
        
        bool  equilibria:: validate( array<double> &C )
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
            std::cerr << "F=" << F << std::endl;
            
#if 0
            int alpha = 0;
            while( alpha <= 40 && !accept_coef(++alpha, F) )
                ;
            std::cerr << "alpha=" << alpha << std::endl;
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
            
            
            
            
            return false;
            
            
            return true;
        }
        
        
    }
}
