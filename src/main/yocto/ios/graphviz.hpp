#ifndef YOCTO_IOS_GRAPHVIZ_INCLUDED
#define YOCTO_IOS_GRAPHVIZ_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace ios
    {
        //! try to tender and remove dotfile
        void   graphviz_render(const string &dotfile,const bool delete_upon_success=true);
        void   graphviz_render(const char   *dotfile,const bool delete_upon_success=true);
        void   graphviz_encode(const char    C, ios::ostream &fp);
        void   graphviz_encode(const char   *s, ios::ostream &fp);
        void   graphviz_encode(const string &s, ios::ostream &fp);

    }
}

#endif
