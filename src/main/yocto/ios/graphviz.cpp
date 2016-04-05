#include "yocto/ios/graphviz.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/code/utils.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

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


        string graphviz_encode(const string &label)
        {
            static const char bad[] = { 0x22, 0x5C, 0x00};
            string ans;
            for(size_t i=0;i<label.size();++i)
            {
                const uint8_t C(label[i]);
                if(C<32||C>126|| 0 != strchr(bad,char(C)) )
                {
                    ans.append('x');
                    ans.append( hexa_text[C] );
                }
                else
                {
                    ans.append(char(C));
                }
            }
            return ans;
        }

    }
}
