#ifndef YOCTO_RTLD_PLUGIN_INCLUDED
#define YOCTO_RTLD_PLUGIN_INCLUDED 1

#include "yocto/rtld/interface.hpp"

namespace yocto
{

    class plugin_info
    {
    public:
        const string soname;
        virtual ~plugin_info() throw();

    protected:
        // make the soname from the module name : "lib" + module_name + "." + YOCTO_SO_EXT
        explicit plugin_info(const string &module_name);
        explicit plugin_info(const char   *module_name);
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(plugin_info);

    };

    template <typename C_API>
    class plugin :  public plugin_info, public interface<C_API>
    {
    public:

        explicit plugin(const string &module_name) :
        plugin_info(module_name),
        interface<C_API>(soname)
        {
        }

        explicit plugin(const char *module_name) :
        plugin_info(module_name),
        interface<C_API>(soname)
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
