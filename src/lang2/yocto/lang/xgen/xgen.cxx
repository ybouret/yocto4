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
            xprs(0),
            name(0)
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
                name = xprs->grammar::name.c_str();

                //______________________________________________________________
                //
                // First Pass: Study Top Level rules
                //______________________________________________________________
                for(ch=ch->next;ch;ch=ch->next)
                {
                    std::cerr << "\t" << ch->label << std::endl;

                    if("RULE"==ch->label)
                    {
                        process_rule_level1(ch);
                        continue;
                    }

                    if("LXR"==ch->label)
                    {
                        process_lxr__level1(ch);
                        continue;
                    }

                    throw exception("%s: unexpected top level '%s'", name, ch->label.c_str());
                }

            }



            
        }
        
    }
}
