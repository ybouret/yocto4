#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::algebra<double> mkl;
        
        bool equilibria:: is_valid( const array<double> &C ) throw()
        {
            assert(C.size()>=M);
            bool flag = true;
            for(size_t i=M;i>0;--i)
            {
                if(C[i]<0)
                {
                    Cbad[i] = true;
                    flag    = false;
                }
                else
                {
                    Cbad[i] = false;
                }
            }
            return flag;
        }

        
        void equilibria:: compute_corrector( const array<double> &C ) throw()
        {
            assert(C.size()>=M);

            //__________________________________________________________________
            //
            // in Ctry used as temporary: the indicator
            //__________________________________________________________________
            for(size_t i=M;i>0;--i)
            {
                const double C_i = C[i];
                Ctmp[i] = (C_i >= 0 ) ? 0.0 : -C_i;
                Ctmp[i] = (C_i >= 0 ) ? 0.0 : 1.0;

            }
            std::cerr << "Ctmp=" << Ctmp << std::endl;
            
            //__________________________________________________________________
            //
            // compute xi = Nu * Ctmp
            //__________________________________________________________________
            mkl::mul(xi,Nu,Ctmp);
            
            //__________________________________________________________________
            //
            // compute dC = Nu' * xi
            //__________________________________________________________________
            mkl::mul_trn(dC, Nu, xi);
            
        }

    }
}
