#include "yocto/math/kernel/det.hpp"

namespace yocto
{
    
    namespace math
    {
        
        namespace kernel
        {
            
            
            
            ptrdiff_t i_determinant_of( matrix<ptrdiff_t> &M ) throw()
            {
                assert(M.is_square());
                assert(M.rows>0);
                
                bool         s = false;
                const size_t n = M.rows;
                for(size_t i=1,i1=i+1;i<=n;++i,++i1)
                {
                    //__________________________________________________________
                    //
                    // find max pivot
                    // if i==n, just check no singular matrix
                    //__________________________________________________________
                    ptrdiff_t               piv = Fabs(M[i][i]);
                    register size_t  ipv = i;
                    for(register size_t k=i1;k<=n;++k)
                    {
                        const ptrdiff_t tmp = Fabs(M[k][i]);
                        if( tmp > piv )
                        {
                            piv = tmp;
                            ipv = k;
                        }
                    }
                    if(piv<=0)
                    {
                        return 0; // singular matrix
                    }
                    
                    //__________________________________________________________
                    //
                    // set the pivot row
                    //__________________________________________________________
                    if(ipv!=i)
                    {
                        s = !s;
                        M.swap_rows(ipv,i);
                    }
                    const array<ptrdiff_t> &Mi    = M[i];
                    const ptrdiff_t         pivot = Mi[i];
                    
                    for(size_t k=i1;k<=n;++k)
                    {
                        array<ptrdiff_t> &Mk    = M[k];
                        const ptrdiff_t   coeff = Mk[i];
                        
                        std::cerr << "M[" << i << "]=" << Mi << std::endl;
                        std::cerr << "pivot=" << pivot << std::endl;
                        std::cerr << "Mk=" << Mk << std::endl;
                        std::cerr << "coeff=" << coeff << std::endl;
                        
                        Mk[i] = 0;
                        // first pass
                        for(size_t j=i1;j<=n;++j)
                        {
                            //Mk[j] -= (Mi[j]*coeff)/pivot;
                            Mk[j] = pivot * Mk[j] - coeff * Mi[j];
                        }
                        std::cerr << "Mnew=" << Mk << " / " << pivot << std::endl;
                        exit(0);

                    }
                }
                
                //__________________________________________________________________
                //
                // Compute determinant
                //__________________________________________________________________
                ptrdiff_t d = 1;
                for(size_t i=n;i>0;--i)
                {
                    d *= M[i][i];
                }
                
                return s ? -d : d;
            }
            
        }
        
    }
    
}
