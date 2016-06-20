#include "yocto/seem/compiler.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    namespace Seem
    {
        Compiler:: ~Compiler() throw()
        {
        }

        Compiler:: Compiler(const bool emitFiles) :
        parser(emitFiles),
        walker( * parser.gram )
        {
        }


        void Compiler:: compile( ios::istream &fp )
        {
            parser.impl->restart();
            auto_ptr<XNode> tree( parser.impl->parse(fp) );

            {
                const string dotname = parser.gram->name + "_output.dot";
                tree->graphviz(dotname);
                ios::graphviz_render(dotname);
            }
            
        }


    }
}
