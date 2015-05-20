#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"
#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            void xgen::grow(logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);

                //______________________________________________________________
                //
                // SUB
                //______________________________________________________________
                if("SUB"==sub->label)
                {
                    const xnode::leaves &children = sub->children(); assert(children.size>0);
                    const xnode         *ch       = children.head;
                    if("ALT"==ch->label)
                    {
                        //______________________________________________________
                        //
                        // standard ALT
                        //______________________________________________________
                        std::cerr << "\t+ALT" << std::endl;
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // standard SUB
                        //______________________________________________________
                        std::cerr << "\t+SUB" << std::endl;
                        for(;ch;ch=ch->next)
                        {
                            std::cerr << "\t\t" << ch->label << std::endl;
                            grow(parent,ch);
                        }
                    }

                    return;
                }


                //______________________________________________________________
                //
                // RAW
                //______________________________________________________________
                if("RAW"==sub->label)
                {
                    const string expr = sub->content();
                    std::cerr << "\t\t|_'" << expr << "'" << std::endl;
                    const string label = "$" + expr;

                    term_ptr *ppT = raw.search(label);
                    if(!ppT)
                    {
                        register_raw(label, expr);
                        ppT = raw.search(label);
                        if(!ppT) throw exception("%s: unexpected missing '%s'", name, label.c_str());
                    }
                    assert(ppT);
                    parent->append( &(**ppT) );
                    return;
                }

                //______________________________________________________________
                //
                // ID
                //______________________________________________________________
                if("ID"==sub->label)
                {
                    const string ruleID = sub->content();
                    std::cerr << "\t\t|_" << ruleID << std::endl;

                    // may be a standard rule
                    agg_ptr *ppA = agg.search(ruleID);
                    if(ppA)
                    {
                        parent->append( &(**ppA) );
                        return;
                    }

                    // may be an alias RegExp
                    term_ptr *ppT = rxp.search(ruleID);
                    if(ppT)
                    {
                        parent->append( &(**ppT) );
                        return;
                    }

                    // may be an alias RawTxt
                    ppT = raw.search(ruleID);
                    if(ppT)
                    {
                        parent->append( &(**ppT) );
                        return;
                    }

                    throw exception("%s: undeclared rule '%s'",name, ruleID.c_str());
                }

                //______________________________________________________________
                //
                // ITEM
                //______________________________________________________________
                if("ITEM"==sub->label)
                {
                    grow_item(parent,sub);
                    return;
                }

                return;
                throw exception("%s: unexpected '%s'", name, sub->label.c_str());
                
                
            }

            void     xgen:: grow_item( logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);
                assert("ITEM"==sub->label);
                assert(2==sub->children().size);

                logical *itm = new_sub();
                grow(itm,sub->children().head);
                const string &kind = sub->children().tail->label;

                switch(kmph(kind))
                {
                    case 0: assert("?"==kind); *parent << xprs->opt(*itm);          break;
                    case 1: assert("*"==kind); *parent << xprs->zero_or_more(*itm); break;
                    case 2: assert("+"==kind); *parent << xprs->one_or_more(*itm);  break;
                    default:
                        assert(-1==kmph(kind));
                        throw exception("%s: unexpected item kind '%s'",name, kind.c_str());
                }
            }


        }

    }
    
}

