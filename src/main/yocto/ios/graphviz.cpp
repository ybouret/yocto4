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

        void graphviz_encode(const char c, ios::ostream &fp )
        {
            const uint8_t C(c);
            if(C>=127)
            {
                fp << 'x' << hexa_text[C];
            }
            else
            {
                if(C<32)
                {
                    switch(C)
                    {
                        case '\n': fp << "\\\\n"; break;
                        case '\r': fp << "\\\\r"; break;
                        case '\t': fp << "\\\\t"; break;
                        default:
                            fp << 'x' << hexa_text[C];
                    }
                }
                else
                {
                    // C>=32&&C<127
                    switch(c)
                    {
                        case '\\': fp << "\\\\";   break;
                        case '\"': fp << "\\\""; break;
                        default:
                            fp << c;
                    }
                }
            }


        }

        void graphviz_encode(const char *label, ios::ostream &fp)
        {
            const size_t n = length_of(label);
            for(size_t i=0;i<n;++i)
            {
                graphviz_encode(label[i],fp);
            }
        }

        void graphviz_encode(const string &label, ios::ostream &fp)
        {
            const size_t n=label.size();
            for(size_t i=0;i<n;++i)
            {
                graphviz_encode(label[i],fp);
            }
        }

    }
}
