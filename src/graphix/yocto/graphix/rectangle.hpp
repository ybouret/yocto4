#ifndef YOCTO_GRAPHIX_RECTANGLE_INCLUDED
#define YOCTO_GRAPHIX_RECTANBLE_INCLUDED 1

#include "yocto/graphix/types.hpp"

namespace yocto
{
    namespace graphix
    {

        class rectangle : public patch
        {
        public:

            explicit rectangle(unit_t X,unit_t Y,unit_t W,unit_t H);
            virtual ~rectangle() throw();

            rectangle(const rectangle &rect) throw();
            

        private:
            YOCTO_DISABLE_ASSIGN(rectangle);
        };

    }

}


#endif

