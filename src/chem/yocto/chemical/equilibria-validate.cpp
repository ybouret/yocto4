#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

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
        
        
        
        
        bool  equilibria:: validate( array<double> &C )
        {
            
            assert(C.size()>=M);
            
            vector<size_t> bad(M,as_capacity);
            vector<size_t> rxn(N,as_capacity);
            
            for(size_t count=1;count<=2;++count)
            {
               
                //______________________________________________________________
                //
                // find bad concentrations
                //______________________________________________________________
                bad.free();
                for(size_t j=1;j<=M;++j)
                {
                    if(active[j]>0 && (C[j]<0))
                        bad.push_back(j);
                }
                std::cerr << "bad=" << bad << std::endl;
                
                const size_t B = bad.size();
                if(B<=0)
                    return true;
                
                //______________________________________________________________
                //
                // find extent limits
                //______________________________________________________________
                find_limits_of(C);
                
                //______________________________________________________________
                //
                // find not blocked reactions
                //______________________________________________________________
                rxn.free();
                for(size_t i=1;i<=N;++i)
                {
                    if( ! limits[i].blocked )
                    {
                        rxn.push_back(i);
                    }
                }
                std::cerr << "rxn=" << rxn << std::endl;
                const size_t R = rxn.size();
                if(R<=0)
                {
                    return false;
                }
                
                
                // find variation according to Nu'
                assert(N>0);
                matrix_t     mu(B,R);
                for(size_t k=1;k<=B;++k)
                {
                    const size_t i = bad[k];
                    for(size_t j=1;j<=R;++j)
                    {
                        mu[k][j] = Nu[rxn[j]][i];
                    }
                }
                std::cerr << "mu=" << mu << std::endl;
                matrix_t mu2(B,B);
                mkl::mul_rtrn(mu2, mu, mu);
                std::cerr << "mu2=" << mu2 << std::endl;
                lu_t     solver(B);
                if(!solver.build(mu2))
                {
                    std::cerr << "-- validate: invalid subsystem" << std::endl;
                    return 0;
                }
                vector_t lam(B,0.0);
                for(size_t k=1;k<=B;++k)
                {
                    lam[k] = -C[ bad[k] ];
                }
                lu_t::solve(mu2, lam);
                std::cerr << "lam=" << lam << std::endl;
                vector_t     Xi(R,0.0);
                mkl::mul_trn(Xi, mu, lam);
                std::cerr << "Xi=" << Xi << std::endl;
                mkl::set(xi,0);
                for(size_t k=1;k<=R;++k)
                {
                    xi[rxn[k]] = Xi[k];
                }
                std::cerr << "xi=" << xi << std::endl;
                clip_extents();
                mkl::mul_trn(dC, Nu, xi);
                std::cerr << "dC=" << dC << std::endl;
                
                // carefull addition
                for(size_t j=M;j>0;--j)
                {
                    double &Cj = C[j];
                    if(Cj>=0)
                    {
                        Cj = max_of<double>(0.0,Cj + dC[j]);
                    }
                    else
                    {
                        Cj += dC[j];
                    }
                }
                std::cerr << "Cnew=" << C << std::endl;
                
            }
            
            return true;
        }
        
        
    }
}
