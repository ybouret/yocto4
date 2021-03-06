#include "yocto/parallel/patch.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
    namespace parallel
    {


        patch_info:: ~patch_info() throw()
        {
        }

        patch_info:: patch_info(const size_t d) throw() :
        dim(d)
        {
            assert(dim==1||dim==2||dim==3);
        }

        patch_info::patch_info(const patch_info &other) throw() :
        dim(other.dim)
        {

        }


        size_t patch_info:: setup(const void  *lower_addr,
                                  const void  *upper_addr,
                                  const void  *pitch_addr,
                                  const void  *width_addr) throw()

        {
            unit_t *lower = (unit_t *)lower_addr;
            unit_t *upper = (unit_t *)upper_addr;
            unit_t *width = (unit_t *)width_addr;
            unit_t *pitch = (unit_t *)pitch_addr;

            size_t ans   = 1;
            for( size_t i=0; i < dim; ++i )
            {
                unit_t &L = lower[i];
                unit_t &U = upper[i];
                unit_t &W = width[i];
                if(L>U) cswap(L,U);
                W=(U+1)-L;
                ans *= W;
            }

            assert(ans>0);
            pitch[0] = 1;
            for( size_t i=1;i<dim; ++i )
            {
                pitch[i] = pitch[i-1] * width[i-1];
            }
            return ans;
        }
        
    }
    
}
