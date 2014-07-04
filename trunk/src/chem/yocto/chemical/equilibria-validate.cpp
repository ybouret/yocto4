#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::algebra<double> mkl;
        
        
        
        void  equilibria:: limits_of(const array<double> &C) throw()
        {
            assert(C.size()>=M);
            
            for(size_t i=N;i>0;--i)
            {
                const array<double> &nu = Nu[i];
                double the_min = 0;
                double the_max = 0;
                bool   got_min = false;
                bool   got_max = false;
                
                
                for(size_t j=M;j>0;--j)
                {
                    const int    nu_j = int(nu[j]);
                    const double C_j  = C[j];
                    
                    //__________________________________________________________
                    //
                    // we must have C_j + nu *xi >= 0
                    //__________________________________________________________
                    
                    
                    if(nu_j>0)
                    {
                        const double value = -C_j/nu_j;
                        if(got_min)
                        {
                            the_min = max_of<double>(-C_j/nu_j,the_min);
                        }
                        else
                        {
                            got_min = true;
                            the_min = value;
                        }
                        continue;
                    }
                    
                    if(nu_j<0)
                    {
                        const double value = C_j/(-nu_j);
                        if(got_max)
                        {
                            the_max = min_of<double>(value,the_max);
                        }
                        else
                        {
                            got_max = true;
                            the_max = value;
                        }
                        continue;
                    }
                    
                    assert(0==nu_j);
                }
                
                has_min[i] = got_min;
                xi_min[i]  = the_min;
                has_max[i] = got_max;
                xi_max[i]  = the_max;
            }
            
        }
        
        
        void equilibria:: correct_xi() throw()
        {
            std::cerr << "xi0=" << xi << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                double &Xi= xi[i];
                std::cerr << "#" << i;
                if(has_min[i])
                {
                    std::cerr << "\tmin=" << xi_min[i];
                    Xi = max_of<double>(Xi,xi_min[i]);
                }
                
                if(has_max[i])
                {
                    std::cerr << "\tmax=" << xi_max[i];
                    Xi = min_of<double>(Xi,xi_max[i]);
                }
                
                std::cerr << std::endl;
            }
            std::cerr << "xi1=" << xi << std::endl;
            
        }
        
        
        void equilibria:: validate( array<double> &C )
        {
            assert(C.size()>=M);
            
            bool must_correct = false;
            for(size_t i=M;i>0;--i)
            {
                if(C[i]<0)
                {
                    must_correct = true;
                    break;
                }
            }
            
            if(!must_correct)
                return;
            
            if(N>0)
            {
                //__________________________________________________________________
                //
                // find out the limits
                //__________________________________________________________________
                limits_of(C);
                
                //__________________________________________________________________
                //
                // try not to move
                //__________________________________________________________________
                mkl::set(xi,0);
                
                //__________________________________________________________________
                //
                // find what must be done
                //__________________________________________________________________
                correct_xi();
                
                //__________________________________________________________________
                //
                // compute the corrected concentrations
                //__________________________________________________________________
                mkl::muladd_trn(C, Nu, xi);
                std::cerr << "Corr=" << C << std::endl;
            }
            
            
        }
        
    }
}
