#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/jacobi.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/cholesky.hpp"

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
            std::cerr << "xi_init=" << xi << ";" << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                const extent &ex = limits[i];
                double       &Xi = xi[i];
                std::cerr << "\tEq" << i << ": ";
                
                if(ex.has_forward_limit)
                {
                    std::cerr << "\tmax_fwd=" << ex.max_forward_value;
                    const double top = ex.max_forward_value;
                    if(Xi>0 && Xi>top)
                    {
                        Xi=top;
                    }
                }
                
                
                if(ex.has_reverse_limit)
                {
                    std::cerr << "\tmax_rev=" << ex.max_reverse_value;
                    const double top = ex.max_reverse_value;
                    if(Xi<0 && (-Xi)>top)
                    {
                        Xi = -top;
                    }
                }
                
                if( ex.blocked )
                {
                    std::cerr << " | BLOCKED";
                    Xi = 0.0;
                }
                std::cerr << std::endl;
            }
            std::cerr << "xi_clip=" << xi << ";" << std::endl;
        }
        
        
#if 0
        static inline double __rint(double x) throw() { return floor(x+0.5); }
        
        
        static inline bool accept_coef(const unsigned alpha, const matrix_t &J) throw()
        {
            assert(alpha>0);
            assert(J.is_square());
            const size_t n = J.rows;
            //std::cerr << "\talpha=" << alpha << std::endl;
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=i;j>0;--j)
                {
                    const double f     = fabs(J[i][j]);
                    const double af    = alpha*f;
                    const double g     = __rint(af);
                    const double rf    = fabs(alpha * (af - g));
                    const int    r     = int(__rint(rf));
                    //std::cerr << "\t\tf=" << f << " -> g=" << g << " -> rf=" << rf << "-> r=" << r << std::endl;
                    if(r>0)
                        return false;
                }
            }
            //std::cerr << "\tok" << std::endl;
            
            return true;
        }
#endif
        
        
        bool  equilibria:: validate( array<double> &C )
        {
            assert(C.size()>=M);
            
            while(true)
            {
                //______________________________________________________________
                //
                // find bad species
                //______________________________________________________________
                bad.free();
                for(size_t j=1;j<=M;++j)
                {
                    if( (active[j]>0) && (C[j]<0) )
                    {
                        bad.push_back(j);
                        dC[j] = -C[j];
                    }
                }
                const size_t B = bad.size();
                if(B<=0)
                {
                    //__________________________________________________________
                    //
                    // everything is fine !
                    //__________________________________________________________
                    return true;
                }
                std::cerr << "bad=" << bad << std::endl;
                
                //______________________________________________________________
                //
                // find online reactions from limits
                //______________________________________________________________
                find_limits_of(C);
                show_limits();
                
                online.free();
                for(size_t i=1;i<=N;++i)
                {
                    if( !limits[i].blocked )
                        online.push_back(i);
                }
                const size_t Q = online.size();
                if(Q<=0)
                {
                    std::cerr << "-- Validate: no available reactions" << std::endl;
                    return false;
                }
                std::cerr << "online=" << online << std::endl;
                
                //______________________________________________________________
                //
                // prepare the Lagrange multipliers
                //______________________________________________________________
                vector_t lambda(B,0.0);
                for(size_t i=B;i>0;--i)
                {
                    lambda[i] = -C[ bad[i] ];
                }
                
                //______________________________________________________________
                //
                // build the expansion matrix from Nu'
                //______________________________________________________________
                matrix_t beta(B,Q);
                for(size_t j=1;j<=B;++j)
                {
                    const size_t jj = bad[j];
                    for(size_t i=1;i<=Q;++i)
                    {
                        const size_t ii = online[i];
                        beta[j][i] = Nu[ii][jj];
                    }
                }
                
                //______________________________________________________________
                //
                // Find the Lagrange multipliers
                //______________________________________________________________
                {
                    
                    std::cerr << "beta=" << beta << std::endl;
                    matrix_t J(B,B);
                    mkl::mul_rtrn(J, beta, beta);
                    std::cerr << "J=" << J <<std::endl;
                    lu_t solver(B);
                    if(!solver.build(J))
                    {
                        std::cerr << "-- Validate: singular sub-system" << std::endl;
                        return false;
                    }
                    std::cerr << "deltaC=" << lambda << std::endl;
                    solver.solve(J,lambda);
                    std::cerr << "lambda=" << lambda << std::endl;
                }
                
                
                //______________________________________________________________
                //
                // compute xi from beta and lambda
                //______________________________________________________________
                for(size_t i=N;i>0;--i) xi[i] = 0;
                for(size_t i=Q;i>0;--i)
                {
                    double sum = 0;
                    for(size_t j=B;j>0;--j)
                    {
                        sum += beta[j][i] * lambda[j];
                    }
                    xi[ online[i] ] = sum;
                }
                std::cerr << "xi=" << xi << std::endl;
                
                //______________________________________________________________
                //
                // clip extents
                //______________________________________________________________
                clip_extents();
                
                //______________________________________________________________
                //
                // compute dC*alpha = Nu'*xi*alpha
                //______________________________________________________________
                
                mkl::mul_trn(dC, Nu, xi);
                std::cerr << "C0=" << C << std::endl;
                std::cerr << "dC=" << dC <<  std::endl;
                
                //______________________________________________________________
                //
                // add carefully:
                // a negative concentration can never become strictly positive
                // a positive concentration can never become striclty negative
                // since xi is optimized and clipped
                //______________________________________________________________
                for(size_t j=M;j>0;--j)
                {
                    if(active[j]>0)
                    {
                        if(C[j]<0)
                        {
                            C[j] += dC[j];
                            C[j]  = min_of<double>(0.0,C[j]);
                        }
                        else
                        {
                            C[j] += dC[j];
                            C[j]  = max_of<double>(0.0,C[j]);
                        }
                    }
                    else
                    {
                        assert(fabs(dC[j])<=0);
                    }
                }
                std::cerr << "C=" << C << std::endl;
            }
            
        }
        
        
    }
}
