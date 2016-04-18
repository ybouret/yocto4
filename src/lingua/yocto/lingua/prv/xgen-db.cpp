#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            aggregate & xgen:: fetch(const string &label)
            {
                
                agg_ptr *ppAgg = adb.search(label);
                if(ppAgg)
                {
                    std::cerr << "returning '" << label << "'" << std::endl;
                    return **ppAgg;
                }
                else
                {
                    aggregate &r = xprs->agg(label);
                    
                    const agg_ptr tmp(&r);
                    if(! adb.insert(tmp) )
                    {
                        throw exception("unexpected multiple '%s'", label.c_str());
                    }
                    std::cerr << "creating  '" << label << "'" << std::endl;
                    return r;
                }
                
            }
            

            void xgen::create_rule( const xnode *top )
            {
                assert("RULE"==top->label());
                assert(false==top->terminal);
                const xnode *node = top->ch->head;
                
                //-- get the rule label from "ID"
                assert("ID"==node->label());
                assert(true==node->terminal);
                const string r_label = node->lx->to_string();
                aggregate   &r       = fetch(r_label);
                grow(r,top->next);
                
            }
            
            
            void xgen::grow(compound &parent, const xnode *node)
            {
                
            }
            
        }

    }

}

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            void xgen::create_lxr_( const xnode *top )
            {
                assert("LXR"==top->label());
                assert(false==top->terminal);
                const xnode *node = top->ch->head;
                
                assert("LX"==node->label());
                assert(true==node->terminal);
            }
            
        }
        
    }
    
}