#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"
#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            xgen:: ~xgen() throw()
            {
            }

            xgen:: xgen(const xnode *node) :
            root(node),
            xprs(0)
            {
                assert(root);
                assert(!root->terminal);
                assert("parser"==root->label);

                //______________________________________________________________
                //
                // Find the parser name and create it
                //______________________________________________________________
                const xnode::leaves &children    = root->children();  assert(children.size>0);
                const xnode         *ch          = children.head;     assert(ch); assert("NAME"==ch->label);
                const string         parser_name = ch->content(1,0);
                const string         parser_main = "main";
                xprs.reset( new parser(parser_name,parser_main) );

                std::cerr << "\t### <" << parser_name << "> ###" << std::endl;

                //______________________________________________________________
                //
                // First Pass: Study Top Level rules
                //______________________________________________________________
                

            }



            
        }
        
    }
}
