#include "yocto/lingua/syntax/term.hpp"
#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            terminal:: ~terminal() throw()
            {
            }

            terminal:: terminal(const string &id) :
            rule(id,UUID)
            {
                YOCTO_LINGUA_PATTERN_IS(terminal);
            }


            void terminal:: viz(ios::ostream &fp) const
            {
                const rule *addr = this;
                fp.viz(addr);
                fp << "[label=\"";
                ios::graphviz_encode(label,fp);
                fp << "\",shape=box";

                fp << "];\n";
            }

        }
    }
}