#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/sort/index.hpp"

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
        
        
        static int compare_abs(const double lhs, const double rhs) throw()
        {
            const double L = fabs(lhs);
            const double R = fabs(rhs);
            return ((L<R) ? -1 : ( (R<L) ? 1 : 0));
        }
        
        bool  equilibria:: validate( array<double> &C )
        {
            
            assert(C.size()>=M);
            
            find_limits_of(C);
            
            matrix_t U(M,M);
            mkl::mul_ltrn(U, Nu, Nu);
            matrix_t V(M,M);
            vector_t w(M,0);
            vector<size_t> iw(M,0);
            vector_t lam(M,0);
            
            std::cerr << "G=" << U << std::endl;

            if( ! math::svd<double>::build(U, w, V) )
            {
                std::cerr << "cannot SVD flow" << std::endl;
                return false;
            }
            make_index(w, iw, compare_abs);
            std::cerr << "U=" << U << std::endl;
            std::cerr << "V=" << V << std::endl;
            std::cerr << "w=" << w << std::endl;
            std::cerr << "iw=" << iw << std::endl;
            for(size_t i=1;i<=(M-N);++i) w[iw[i]] = 0;
            std::cerr << "w=" << w << std::endl;
            {
                size_t nbad = 0;
                for(size_t j=1;j<=M;++j)
                {
                    dC[j] = 0;
                    if( active[j] && (C[j]<0) )
                    {
                        ++nbad;
                        dC[j] = -C[j];
                    }
                }
                
                if(nbad<=0)
                    return true;
                
                std::cerr << "dC=" << dC << std::endl;
                math::svd<double>::solve(U, w, V, dC, lam);
                std::cerr << "lam=" << lam << std::endl;
                mkl::mul(xi, Nu, lam);
                std::cerr << "xi=" << xi << std::endl;
                clip_extents();
                mkl::mul_trn(dC, Nu, xi);
                std::cerr << "dC=" << dC << std::endl;
            }
            
            
            return true;
        }
        
        
    }
}
