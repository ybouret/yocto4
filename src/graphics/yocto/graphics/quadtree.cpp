#include "yocto/graphics/quadtree.hpp"
#include <cstring>

namespace yocto
{
    namespace graphics
    {

        Quad:: Quad(const unit_t X, const unit_t Y, const unit_t W, const unit_t H, const int Level) :
        rectangle(X,Y,W,H),
        parent(0),
        sub(),
        level(Level)
        {
            memset(sub,0,sizeof(sub));
        }

        Quad:: ~Quad() throw()
        {

        }

        void Quad::subdivide()
        {
            
        }

    }
}

