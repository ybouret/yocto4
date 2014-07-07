#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::algebra<double> mkl;
        
        
        
        size_t  equilibria:: limits_of(const array<double> &C) throw()
        {
            assert(C.size()>=M);
            
            size_t count = 0;
            for(size_t i=N;i>0;--i)
            {
                const array<double> &nu = Nu[i];
                double the_min = 0;
                double the_max = 0;
                bool   got_min = false;
                bool   got_max = false;
                online[i]      = true;
                
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
                            the_min = max_of<double>(value,the_min);
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
                
                ++count;
                if(got_max&&got_min)
                {
                    // special case: block the reaction
                    if(the_max<=the_min)
                    {
                        the_max = the_min = 0;
                        online[i] = false;
                        --count;
                    }
                }
                
                has_min[i] = got_min;
                xi_min[i]  = the_min;
                has_max[i] = got_max;
                xi_max[i]  = the_max;
            }
            return count;
        }
        
        
        void equilibria:: correct_xi() throw()
        {
            std::cerr << "xi0=" << xi << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                double &Xi= xi[i];
                std::cerr << "#" << i;
                std::cerr << " " << (online[i]? " ON: " : "OFF:");
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
        
        bool equilibria:: must_correct( const array<double> &C ) const throw()
        {
            assert(C.size()>=M);
            
            for(size_t i=M;i>0;--i)
            {
                if( active[i] && (C[i]<0) )
                {
                    assert(N>0);
                    return true;
                }
            }
            return false;
        }
        
        
        void equilibria:: validate( array<double> &C )
        {
            assert(C.size()>=M);
            
            if(!must_correct(C))
                return;
            
            if(N>0)
            {
                //______________________________________________________________
                //
                // find out the limits
                //______________________________________________________________
                if(limits_of(C)<=0)
                    throw exception("equilibria.validate(no active reaction)");
                
                //______________________________________________________________
                //
                // try not to move
                //______________________________________________________________
                mkl::set(xi,0);
                
                //______________________________________________________________
                //
                // find what must be done
                //______________________________________________________________
                correct_xi();
                
                //______________________________________________________________
                //
                // compute the corrected concentrations
                //______________________________________________________________
                mkl::muladd_trn(C, Nu, xi);
                
                //______________________________________________________________
                //
                // correct roundoff for active reactions
                //______________________________________________________________
                for(size_t i=N;i>0;--i)
                {
                    if(online[i])
                    {
                        // this reaction is active
                        const array<double> &nu = Nu[i];
                        for(size_t j=M;j>0;--j)
                        {
                            if( (0!=int(nu[j])) && (C[j]<0.0) )
                            {
                                assert(active[j]);
                                C[j] = 0.0;
                            }
                        }
                    }
                }
                std::cerr << "Corr=" << C << std::endl;
            }
            
            
        }
        
    }
}
