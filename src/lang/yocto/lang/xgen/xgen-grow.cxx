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
                
                
                switch(gmph(sub->label))
                {
                    case 0: grow_sub(parent,sub); break;
                    case 1: grow_alt(parent,sub); break;
                    case 2: grow_raw(parent,sub); break;
                    case 3: grow_rxp(parent,sub); break;
                    case 4: grow_id( parent,sub); break;
                    case 5: grow_itm(parent,sub); break;
                    default:
                        assert(-1==gmph(sub->label));
                        throw exception("%s: unexpected '%s'", name, sub->label.c_str());
                }
                
                
            }
            
            void     xgen:: grow_sub( logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);
                assert("SUB"==sub->label);
                const xnode::leaves &children = sub->children(); assert(children.size>0);
                const xnode         *ch       = children.head;
                if("ALT"==ch->label)
                {
                    //______________________________________________________
                    //
                    // standard ALT
                    //______________________________________________________
                    logical *alt =  & xprs->alt();
                    for(;ch;ch=ch->next)
                    {
                        grow(alt,ch);
                    }
                    parent->append(alt);
                }
                else
                {
                    //______________________________________________________
                    //
                    // standard SUB
                    //______________________________________________________
                    for(;ch;ch=ch->next)
                    {
                        grow(parent,ch);
                    }
                }
                
            }
            
            
            void     xgen:: grow_itm( logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);
                assert("ITEM"==sub->label);
                assert(2==sub->children().size);
                
                logical *itm = create_sub();
                grow(itm,sub->children().head);
                const string &kind = sub->children().tail->label;
                
                rule *r = itm;
                if(1==itm->size)
                {
                    logical::operand *op = itm->pop_front();
                    r = op->addr;
                    //std::cerr << "should kill " << itm->label << std::endl;
                    delete op;
                    delete_sub(itm->label);
                }
                
                switch(kmph(kind))
                {
                    case 0: assert("?"==kind); *parent << xprs->opt(*r);          break;
                    case 1: assert("*"==kind); *parent << xprs->zero_or_more(*r); break;
                    case 2: assert("+"==kind); *parent << xprs->one_or_more(*r);  break;
                    default:
                        assert(-1==kmph(kind));
                        throw exception("%s: unexpected item kind '%s'",name, kind.c_str());
                }
            }
            
            void xgen:: grow_id( logical *parent, const xnode *sub )
            {
                assert(parent);
                assert(sub);
                assert("ID"==sub->label);
                const string ruleID = sub->content();
                
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
            
            void xgen::grow_rxp(logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);
                assert("RXP"==sub->label);
                
                const string expr   = sub->content();
                const string &label =  expr;
                
                term_ptr *ppT = rxp.search(label);
                if(!ppT)
                {
                    register_rxp(label, expr);
                    ppT = rxp.search(label);
                    if(!ppT) throw exception("%s: unexpected missing \"%s\"", name, label.c_str());
                }
                assert(ppT);
                parent->append( &(**ppT) );
            }
            
            
            void xgen::grow_raw(logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);
                assert("RAW"==sub->label);
                const string  expr  = sub->content();
                const string &label = expr;
                
                term_ptr *ppT = raw.search(label);
                if(!ppT)
                {
                    register_raw(label, expr);
                    ppT = raw.search(label);
                    if(!ppT) throw exception("%s: unexpected missing '%s'", name, label.c_str());
                }
                assert(ppT);
                parent->append( &(**ppT) );
            }
            
            void xgen::grow_alt(logical *parent, const xnode *sub)
            {
                assert(parent);
                assert(sub);
                assert("ALT"==sub->label);
                logical *tmp = create_sub();
                for( const xnode *ch = sub->head(); ch; ch=ch->next)
                {
                    grow(tmp,ch);
                }
                
                if(1==tmp->size)
                {
                    logical::operand *op = tmp->pop_front();
                    parent->append(op->addr);
                    delete op;
                    //std::cerr << "should kill " << tmp->label << std::endl;
                    delete_sub(tmp->label);
                }
                else
                {
                    parent->append(tmp);
                }
            }
            
            
        }
        
    }
    
}

