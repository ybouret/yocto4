#include "yocto/seem/compiler.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/graphviz.hpp"
#include "yocto/ios/ocstream.hpp"

#include <iostream>

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
            walker.on_term( this, & Compiler::on_term );
            walker.on_rule( this, & Compiler::on_rule );
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
            std::cerr << "--Walking..." << std::endl;
            walker.walk( tree.__get(), NULL );
            
        }

    }
}

namespace yocto
{
    namespace Seem
    {
        
#include "seem.def"

        void Compiler:: on_term(const string &label,
                                const string &content)
        {
            std::cerr << "+" << label << "='" << content << "'" << std::endl;

        }

        void Compiler:: on_rule(const string &label,
                                const size_t ns)
        {
            std::cerr << "@" << label << "/" << ns << std::endl;
        }

    }
}
