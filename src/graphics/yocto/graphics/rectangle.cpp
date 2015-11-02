#include "yocto/graphics/rectangle.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace graphix
    {

        rectangle:: ~rectangle() throw()
        {
        }


        static inline unit_t __check_w(const unit_t w)
        {
            if(w<=0) throw exception("rectanle: invalid width");
            return w;
        }

        static inline unit_t __check_h(const unit_t h)
        {
            if(h<=0) throw exception("rectanle: invalid height");
            return h;
        }


        rectangle:: rectangle(unit_t X,unit_t Y,unit_t W,unit_t H) :
        patch( vertex(X,Y), vertex(X+__check_w(W)-1,Y+__check_h(H)-1) )
        {

        }

        rectangle:: rectangle( const rectangle &rect ) throw():
        patch(rect)
        {
        }

    }
}
