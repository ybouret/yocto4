#include "yocto/seem/compiler.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/graphviz.hpp"
#include "yocto/ios/ocstream.hpp"

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
            //__________________________________________________________________
            //
            // registering methods
            //__________________________________________________________________
            walker.on_term("NUMBER", this, & Compiler::OnNumber);
            walker.on_term("ID",     this, & Compiler::OnID);
        }


        void Compiler:: compile( ios::istream &fp )
        {
            //__________________________________________________________________
            //
            // prepare parser
            //__________________________________________________________________
            parser.impl->restart();

            //__________________________________________________________________
            //
            // run parser
            //__________________________________________________________________
            auto_ptr<XNode> tree( parser.impl->parse(fp) );

            {
                const string dotname = parser.gram->name + "_output.dot";
                tree->graphviz(dotname);
                ios::graphviz_render(dotname);
            }

            //__________________________________________________________________
            //
            // compile: walk...
            //__________________________________________________________________
            //ios::ocstream out( ios::cstderr );
            walker.walk( tree.__get(), NULL );
            
        }

    }
}

#include <iostream>
namespace yocto
{
    namespace Seem
    {
        void Compiler::OnNumber(const string &content)
        {
            std::cerr << "pushf " << content << std::endl;
        }

        void Compiler::OnID(const string &content)
        {
            std::cerr << "push '" << content << "'" << std::endl;
        }

        void Compiler:: OnAXP(const size_t ns)
        {
            std::cerr << "axp /" << ns << std::endl;
        }
    }
}
