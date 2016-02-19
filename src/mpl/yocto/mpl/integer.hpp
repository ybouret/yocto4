#ifndef YOCTO_MPL_INTEGER_INCLUDED
#define YOCTO_MPL_INTEGER_INCLUDED 1

#include "yocto/mpl/natural.hpp"

namespace yocto
{
    namespace mpl
    {
        enum sign_type
        {
            is_negative,
            is_zero,
            is_posivite
        };

        class integer : public object
        {
        public:
            const sign_type s;
            const natural   n;

            inline void update() throw()
            {
                YOCTO_CHECK_MPN(n);
                if(n.is_zero()) (sign_type&)s = is_zero;
            }

            //__________________________________________________________________
            //
            // canonical
            //__________________________________________________________________
            inline integer() : s(is_zero), n() {}

        private:

        };

    }

}

#endif

