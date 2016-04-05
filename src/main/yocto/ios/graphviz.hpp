#ifndef YOCTO_IOS_GRAPHVIZ_INCLUDED
#define YOCTO_IOS_GRAPHVIZ_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
    namespace ios
    {
        //! try to tender and remove dotfile
        void   graphviz_render(const string &dotfile,const bool delete_upon_success=true);
        void   graphviz_render(const char   *dotfile,const bool delete_upon_success=true);
        string graphviz_encode(const string &label);

    }
}

#endif
