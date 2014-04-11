#define YOCTO_ZTYPE 's'
#include "extend.cxx"

namespace yocto
{
    namespace math
    {
        extend_mode extend_mode_for_derivative( extend_mode m ) throw()
        {
            switch(m)
            {
                case extend_constant: return extend_constant;
                case extend_cyclic:   return extend_cyclic;
                case extend_even:     return extend_odd;
                case extend_odd:      return extend_even;
            }
        }
        
    }
}

