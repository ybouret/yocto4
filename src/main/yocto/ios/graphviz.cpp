#include "yocto/ios/graphviz.hpp"
#include "yocto/fs/local-fs.hpp"

#include <cstdio>
#include <cstdlib>

namespace yocto
{
    namespace ios
    {
        void graphviz_render(const string &dotfile, const bool delete_upon_success)
        {
            static vfs &fs = local_fs::instance();
            const string pngfile = dotfile;
            vfs::change_extension( (string &)pngfile, "png");
            const string cmd     = "dot -Tpng -o " + pngfile + " " + dotfile;
            if( 0 != system( cmd.c_str() ) )
            {
                return;
            }
            if(delete_upon_success)
            fs.try_remove_file(dotfile);
        }

        void graphviz_render(const char *dotfile, const bool delete_upon_success)
        {
            const string fn(dotfile);
            graphviz_render(fn,delete_upon_success);
        }
    }
}
