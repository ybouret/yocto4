#ifndef YOCTO_RTLD_PLUGIN_INCLUDED
#define YOCTO_RTLD_PLUGIN_INCLUDED 1

#include "yocto/rtld/interface.hpp"

namespace yocto
{

    template <typename C_API>
    class plugin : public module, public interface<C_API>
    {
    public:

        explicit plugin(const string &soname, const string &loader) :
        module( dylib_load( soname.c_str(), dylib_full ) ),
        interface<C_API>(*this)
        {
        }

        virtual ~plugin() throw()
        {
        }
        


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
    };

}


#endif
