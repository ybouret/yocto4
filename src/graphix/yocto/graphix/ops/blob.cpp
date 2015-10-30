#include "yocto/graphix/ops/blob.hpp"

namespace yocto
{
    namespace graphix
    {

        blob:: ~blob() throw()
        {
        }

        blob::blob(size_t W,size_t H) :
        pixmap<type>(W,H),
        delta()
        {
            vertex *v = (vertex *)delta;
            v[0] = vertex(-1,0);
            v[1] = vertex(1,0);
            v[2] = vertex(0,1);
            v[3] = vertex(1,0);

            v[4] = vertex(-1,1);
            v[5] = vertex(-1,-1);
            v[6] = vertex(1,1);
            v[7] = vertex(1,-1);
        }
    }

}
