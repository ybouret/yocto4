#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {
        unsigned rgb_distance_sq( const rgb_t &lhs, const rgb_t &rhs ) throw()
        {
            const int dr = int(lhs.r) - int(rhs.r);
            const int dg = int(lhs.g) - int(rhs.g);
            const int db = int(lhs.b) - int(rhs.b);
            return dr*dr + dg*dg + db*db;
        }
    }
}
