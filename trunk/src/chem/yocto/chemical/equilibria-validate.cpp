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
        // eqs. info
        //______________________________________________________________________

        equilibria:: eqinfo:: eqinfo() throw() :
        hasMin(false),
        xi_min(0.0),
        hasMax(false),
        xi_max(0.0)
        {
        }
        
        equilibria:: eqinfo :: ~eqinfo() throw()
        {}
        
        
        equilibria:: eqinfo:: eqinfo( const eqinfo &other ) throw() :
        hasMin( other.hasMin ),
        xi_min( other.xi_min ),
        hasMax( other.hasMax ),
        xi_max( other.xi_max )
        {
            
        }
        
        //______________________________________________________________________
        //
        // limits
        //______________________________________________________________________

        void equilibria:: find_limits_of(const array<double> &C) throw()
        {
            assert(C.size()>=M);
            for(size_t i=N;i>0;--i)
            {
                eqinfo              &info = limits[i];
                const array<double> &nu   = Nu[i];
                info.hasMin = info.hasMax = false;
                info.xi_min = info.xi_max = 0;
                
                for(size_t j=M;j>0;--j)
                {
                    const int    coef = int(nu[j]);
                    const double Cj   = C[j];
                    if(coef>0)
                    {
                        if(info.hasMin)
                        {
                            info.xi_min = max_of<double>(info.xi_min,-Cj/coef);
                        }
                        else
                        {
                            info.hasMin = true;
                            info.xi_min = -Cj/coef;
                        }
                        continue;
                    }
                    
                    if(coef<0)
                    {
                        if(info.hasMax)
                        {
                            info.xi_max = min_of<double>(info.xi_max,Cj/(-coef));
                        }
                        else
                        {
                            info.hasMax = true;
                            info.xi_max = Cj/(-coef);
                        }
                        continue;
                    }
                    
                    assert(!coef);
                }
            }
            
        }
        
        void  equilibria:: clip_extents() throw()
        {
            std::cerr << "xi_init=" << xi << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                const eqinfo &info = limits[i];
                double       &Xi   = xi[i];
                std::cerr << "eq" << i << ": ";
                if(info.hasMin)
                {
                    std::cerr << "\tmin=" << info.xi_min;
                    Xi = max_of<double>(Xi,info.xi_min);
                }
                
                if(info.hasMax)
                {
                    std::cerr << "\tmax=" << info.xi_max;
                }
                
                std::cerr << std::endl;
            }
            std::cerr << "xi_clip=" << xi << std::endl;
        }

        
        static inline
        size_t count_bad( const array<bool> &active, const array<double> &C, size_t M ) throw()
        {
            size_t nbad = 0;
            for(size_t j=M;j>0;--j)
            {
                if( active[j] && (C[j]<0) )
                {
                    ++nbad;
                }
            }
            return nbad;
        }
        
        bool  equilibria:: validate( array<double> &C ) throw()
        {
            
            assert(C.size()>=M);
            
            find_limits_of(C);
            
            size_t nbad = count_bad(active, C, M);
            while(nbad>0)
            {
                std::cerr << "#bad=" << nbad << std::endl;
                //______________________________________________________________
                //
                // create the concentration vector
                //______________________________________________________________
                for(size_t j=M;j>0;--j)
                {
                    const double Cj = C[j];
                    dC[j] = (Cj < 0) ? -Cj : 0;
                }
                
                std::cerr << "Cbad=" << dC << std::endl;
                //______________________________________________________________
                //
                // create the calibration xi
                //______________________________________________________________
                mkl::mul(xi,Nu,dC);
                std::cerr << "xi_bad=" << xi << std::endl;
                clip_extents();
                //______________________________________________________________
                //
                // create the search direction
                //______________________________________________________________
                mkl::mul_trn(dC, Nu, xi);
                std::cerr << "dC=" << dC << std::endl;
                return false;
            }
            
            
            return true;
        }

        
    }
}
