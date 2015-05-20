#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"
#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            void xgen::process_rule_level1(const xnode *node)
            {
                assert(node);
                assert(!node->terminal);
                //______________________________________________________________
                //
                // take the top level node for the rule content
                //______________________________________________________________
                const xnode::leaves &children = node->children(); assert(children.size==2);
                const xnode         *ch       = children.head;    assert(ch); assert("ID"==ch->label);
                const string         ID       = ch->content();

                std::cerr << "\t\tID=" << ID << std::endl;

                if(xprs->has(ID))
                {
                    throw exception("%s: multiple rule '%s'", name, ID.c_str());
                }

                //______________________________________________________________
                //
                // take the top level node for the rule content,
                // and check if we have an alias
                //______________________________________________________________
                ch=ch->next;
                assert("SUB"==ch->label);
                const xnode::leaves &sub = ch->children();
                if(1==sub.size)
                {
                    
                }

                
            }


        }

    }

}
