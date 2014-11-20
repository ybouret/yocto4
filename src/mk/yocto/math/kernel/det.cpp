#include "yocto/math/kernel/det.hpp"
#include "yocto/code/arith.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/math/kernel/tao.hpp"

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
                
                //ptrdiff_t P = 1;
                //ptrdiff_t D = 1;
                
                lw_array<ptrdiff_t> P( M.get_aux<ptrdiff_t>(), n );
                lw_array<ptrdiff_t> D( M.get_aux<ptrdiff_t>(), n );
                
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
                    
                    D[i] = pivot;
                    P[i] = 1;
                    for(size_t k=i1;k<=n;++k)
                    {
                        array<ptrdiff_t> &Mk    = M[k];
                        const ptrdiff_t   coeff = Mk[i];
                        
                        Mk[i] = 0;
                        for(size_t j=i1;j<=n;++j)
                        {
                            //Mk[j] -= (Mi[j]*coeff)/pivot;
                            Mk[j] = pivot * Mk[j] - coeff * Mi[j];
                        }
                        const ptrdiff_t g = tao::simplify(Mk);
                        //std::cerr << "g=" << g << std::endl;
                        D[i] *= g;
                        P[i] *= pivot;
                    }
                    std::cerr << "\tD=" << D << std::endl;
                    std::cerr << "\tP=" << P << std::endl;
                }
                
                //__________________________________________________________________
                //
                // Compute determinant
                //__________________________________________________________________
                std::cerr << "D=" << D << std::endl;
                std::cerr << "P=" << P << std::endl;
                std::cerr << "Mout=" << M << std::endl;
                //D /= P;
                //return s ? -D : D;
                return 0;
            }
            
        }
        
    }
    
}
