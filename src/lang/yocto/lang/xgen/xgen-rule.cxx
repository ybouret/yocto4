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
                assert(2==node->size());
                //______________________________________________________________
                //
                // take the top level node for the rule content
                //______________________________________________________________
                const xnode         *ch       = node->head();    assert(ch); assert("ID"==ch->label);
                const string         ID       = ch->content();


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
                    const xnode *sngl = sub.head;
                    if("RAW"==sngl->label)
                    {
                        const string expr = sngl->content();
                        if( count(sngl->label,expr) != 1 )
                        {
                            throw exception("%s: '%s' is not properly aliased", name, expr.c_str());
                        }
                        register_raw(ID,expr);
                        return;
                    }

                    if("RXP"==sngl->label)
                    {
                        const string expr = sngl->content();
                        if( count(sngl->label,expr) != 1)
                        {
                            throw exception("%s: \"%s\" is not properly aliased", name, expr.c_str());
                        }
                        register_rxp(ID,expr);
                        return;
                    }
                }

                //______________________________________________________________
                //
                // default: create a new aggregate
                //______________________________________________________________
                register_std(ID);

            }



        }
    }
}


namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            void xgen::process_rule_level2(const xnode *node)
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
                agg_ptr             *ppA      = agg.search(ID);
                if(!ppA)
                {
                    return; // already processed !
                }
                logical *parent = &(**ppA);
                curr            = parent;
                indx            = 0;
                grow(parent,ch->next);
            }
            
        }
    }
}