#ifndef YOCTO_GFX_NAMED_RGB_INCLUDED
#define YOCTO_GFX_NAMED_RGB_INCLUDED 1

#include "yocto/gfx/rgb.hpp"
#include "yocto/container/sequence.hpp"

namespace yocto
{
    namespace gfx
    {
        rgb_t   get_named_rgb(const string &id);
        rgb_t   get_named_rgb(const char   *id);
        size_t  get_named_rgb(sequence<string> &names); //!< return the max names[...].size()
        
    }
}

#endif
