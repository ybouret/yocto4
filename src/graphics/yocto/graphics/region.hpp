#ifndef YOCTO_GRAPHICS_REGION_INCLUDED
#define YOCTO_GRAPHICS_REGION_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace graphics
    {

        typedef list<vertex> region_base;
        class region : public region_base
        {
        public:
            explicit region() throw();
            virtual ~region() throw();
            region(const region &);
            region & operator=(const region &);

            

        };

    }
}

#endif

