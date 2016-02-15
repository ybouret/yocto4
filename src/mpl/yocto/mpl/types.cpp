#include "yocto/mpl/types.hpp"
#include <cmath>

namespace yocto
{
    namespace mpl
    {
        const char manager::name[] = "mpl";
        

#define SIN_OF(LN2) sin(6.28318530717958623199592693708837032318115234375/(size_t(1)<<(LN2)))
        const double manager::sin_table[32] =
        {
            SIN_OF(0),  SIN_OF(1),  SIN_OF(2),  SIN_OF(3),  SIN_OF(4),
            SIN_OF(5),  SIN_OF(6),  SIN_OF(7),  SIN_OF(8),  SIN_OF(9),
            SIN_OF(10), SIN_OF(11), SIN_OF(12), SIN_OF(13), SIN_OF(14),
            SIN_OF(15), SIN_OF(16), SIN_OF(17), SIN_OF(18), SIN_OF(19),
            SIN_OF(20), SIN_OF(21), SIN_OF(22), SIN_OF(23), SIN_OF(24),
            SIN_OF(25), SIN_OF(26), SIN_OF(27), SIN_OF(28), SIN_OF(29),
            SIN_OF(30), SIN_OF(31)
        };


        manager:: manager() throw() :
        blocks(max_bytes)
        {
        }

        manager:: ~manager() throw()
        {
        }


    }
}