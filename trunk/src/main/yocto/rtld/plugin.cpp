#include "yocto/rtld/plugin.hpp"

namespace yocto
{

    plugin_info:: ~plugin_info() throw()
    {
    }

    plugin_info:: plugin_info(const string &module_name) :
    soname( "lib" + module_name + "." YOCTO_SO_EXT)
    {
    }

    plugin_info:: plugin_info(const char   *module_name) :
    soname( "lib" + string(module_name) + "." YOCTO_SO_EXT )
    {
    }
    

}