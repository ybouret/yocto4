#ifndef YOCTO_GRAPHICS_SUBPATCH_INCLUDED
#define YOCTO_GRAPHICS_SUBPATCH_INCLUDED 1

#include "yocto/graphics/types.hpp"
#include "yocto/container/xslot.hpp"

namespace yocto
{
    namespace graphics
    {

        class subpatch : public patch, public xslot
        {
        public:
            explicit subpatch(const patch &p);
            virtual ~subpatch() throw();

        private:
            YOCTO_DISABLE_ASSIGN(subpatch);
        };
        
    }
}

#endif
