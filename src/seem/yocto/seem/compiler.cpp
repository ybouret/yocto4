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

#include "yocto/exception.hpp"

namespace yocto
{
    namespace Seem
    {
        
#include "seem.def"

        void Compiler:: on_term(const string &label,
                                const string &content)
        {
            //std::cerr << "+" << label << "='" << content << "'" << std::endl;
            const int code = walker.hash_term(label);
            switch(code)
            {
                case SEEM_NUMBER:
                    std::cerr << "push num " << content << std::endl;
                    break;

                case SEEM_ID:
                    std::cerr << "push id " << content << std::endl;
                    break;


                case SEEM_PLUS:
                case SEEM_MINUS:
                case SEEM_MUL:
                case SEEM_DIV:
                case SEEM_MOD:
                    std::cerr << "push op " << label << std::endl;
                    break;

                default:
                    throw exception("NOT Implemented Term '%s'", label.c_str());
                    break;
            }
        }

        void Compiler:: on_rule(const string &label,
                                const size_t  ns)
        {
            switch(walker.hash_rule(label))
            {
                case SEEM_AXP:
                case SEEM_MXP:
                case SEEM_PXP:
                case SEEM_FUNC:
                    std::cerr << "call " << label << "/" << ns << std::endl;
                    break;

                case SEEM_ARGS: // wrapper for FUNC
                    break;

                default:
                    throw exception("NOT Implemented Rule '%s'", label.c_str());
                    break;
            }
        }

    }
}
