#include "yocto/seem/compiler.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/graphviz.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/osstream.hpp"

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
        walker( * parser.gram ),
        out(0)
        {
            walker.on_term( this, & Compiler::on_term );
            walker.on_rule( this, & Compiler::on_rule );
        }


        void Compiler:: run( ios::ostream &bin, ios::istream &fp )
        {
            out = &bin;
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

        void Compiler:: run( string       &bin, ios::istream &fp )
        {
            bin.clear();
            ios::osstream out(bin);
            run(out,fp);
        }

    }
}

#include "yocto/exception.hpp"
#include "yocto/ios/net-string.hpp"

namespace yocto
{
    namespace Seem
    {
        
#include "seem.def"

        void Compiler:: on_term(const string &label,
                                const string &content)
        {
            //std::cerr << "+" << label << "='" << content << "'" << std::endl;
            assert(out);
            ios::ostream &bin = *out;
            const int code = walker.hash_term(label);
            switch(code)
            {
                case SEEM_NUMBER:
                case SEEM_ID:
                    bin.emit<OpCode>(code);
                    ios::net_string::write(content,bin);
                    std::cerr << "push " << label << " " << content << std::endl;
                    break;


                case SEEM_PLUS:
                case SEEM_MINUS:
                case SEEM_MUL:
                case SEEM_DIV:
                case SEEM_MOD:
                    std::cerr << "push " << label << std::endl;
                    break;

                default:
                    throw exception("NOT Implemented Term '%s'", label.c_str());
                    break;
            }
        }

        void Compiler:: on_rule(const string &label,
                                const size_t  ns)
        {
            assert(out);
            ios::ostream &bin = *out;
            const int    code = walker.hash_rule(label);
            switch(code)
            {
                case SEEM_AXP:
                case SEEM_MXP:
                case SEEM_PXP:
                case SEEM_FUNC:
                case SEEM_ARGS:
                    bin.emit<OpCode>(code);
                    bin.emit<OpCode>(ns);
                    std::cerr << "call " << label << "/" << ns << std::endl;
                    break;
                    

                default:
                    throw exception("NOT Implemented Rule '%s'", label.c_str());
                    break;
            }
        }

    }
}
