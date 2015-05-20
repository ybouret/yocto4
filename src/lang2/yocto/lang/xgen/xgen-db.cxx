#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"
#include "yocto/lang/pattern/logic.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            void xgen:: register_std( const string &label )
            {
                aggregate &r = xprs->agg(label);
                if(!agg.insert(&r))
                {
                    throw exception("%s: unexpected storage failure for ID='%s'",name,label.c_str());
                }
            }

            void xgen:: register_rxp(const string &label, const string &expr)
            {
                terminal &r = xprs->term(label.c_str(),expr.c_str(),standard);
                if(!rxp.insert(&r))
                {
                    throw exception("%s: unexpected storage failure for RegExp \"%s\"",name,label.c_str());
                }
            }

            void xgen:: register_raw(const string &label, const string &expr)
            {

                const lexical::action a( & xprs->scanner, & lexical::scanner::forward );
                pattern              *p = lang::logical::equal(expr);
                xprs->scanner.make(label, p, a);
                terminal &r = xprs->decl_term(label,univocal);
                if(!raw.insert(&r))
                {
                    throw exception("%s: unexpected storage failure for RawTxt '%s'",name,label.c_str());
                }

            }

            size_t   xgen:: count(const string &type, const string &expr) const
            {
                size_t num = 0;
                count_cb(num, type, expr, root);
                return num;
            }

            void xgen:: count_cb(size_t &num, const string &type, const string &expr, const xnode *node) const
            {
                if(node->terminal)
                {
                    if(type==node->label)
                    {
                        const string data = node->content();
                        if(data==expr)
                        {
                            ++num;
                        }
                    }

                }
                else
                {
                    for(const xnode *ch = node->children().head;ch;ch=ch->next)
                    {
                        count_cb(num,type,expr,ch);
                    }
                }
            }
            

        }

    }

}

