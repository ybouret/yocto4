#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            aggregate & xgen:: fetch_agg(const string &label)
            {
                agg_ptr *ppAgg = agg_db.search(label);
                if(ppAgg)
                {
                    std::cerr << "returning '" << label << "'" << std::endl;
                    rule &r = **ppAgg;
                    assert(r.self);
                    assert(aggregate::UUID==r.uuid);
                    return *static_cast<aggregate *>(r.self);
                }
                else
                {
                    aggregate &r = xprs->agg(label);
                    
                    const agg_ptr tmp(&r);
                    if(! agg_db.insert(tmp) )
                    {
                        throw exception("unexpected multiple ID '%s'", label.c_str());
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
                aggregate   &r       = fetch_agg(r_label);
                grow(r,node->next);
                
            }
            
            
            rule & xgen::fetch_rxp(const string &label)
            {
                term_ptr *ppRxp = rxp_db.search(label);
                if(ppRxp)
                {
                    std::cerr << "returning rxp '" << label << "'" << std::endl;
                    return **ppRxp;
                }
                else
                {
                    rule &r = xprs->terminal(label, label);
                    
                    const term_ptr tmp(&r);
                    if(! rxp_db.insert(tmp) )
                    {
                        throw exception("unexpected multiple RXP '%s'", label.c_str());
                        
                    }
                    std::cerr << "creating  rxp '" << label << "'" << std::endl;
                    return r;
                }
            }
            
            rule & xgen::fetch_raw(const string &label)
            {
                term_ptr *ppRxp = raw_db.search(label);
                if(ppRxp)
                {
                    std::cerr << "returning raw '" << label << "'" << std::endl;
                    return **ppRxp;
                }
                else
                {
                    rule &r = xprs->univocal(label);
                    
                    const term_ptr tmp(&r);
                    if(! raw_db.insert(tmp) )
                    {
                        throw exception("unexpected multiple RAW '%s'", label.c_str());
                        
                    }
                    std::cerr << "creating  raw '" << label << "'" << std::endl;
                    return r;
                }
            }
            
            
            rule & __modified(const rule           &r,
                              parser               &prs,
                              const string         &modifier,
                              const hashing::mperf &hmod)
            {
                switch(hmod(modifier))
                {
                    case 0: assert("?"==modifier); return prs.opt(r);
                    case 1: assert("*"==modifier); return prs.zero_or_more(r);
                    case 2: assert("+"==modifier); return prs.one_or_more(r);
                    default: break;
                }
                throw exception("unknown modifier '%s'", modifier.c_str());
            }
            
            void xgen::grow(compound &parent, const xnode *node)
            {
                while(node)
                {
                    switch(hsub(node->label()))
                    {
                        case 0: assert("ID"==node->label());
                        {
                            assert(node->terminal);
                            const string content = node->lx->to_string();
                            std::cerr << "\t" << parent.label << "->" << content << std::endl;
                            parent << fetch_agg(content);
                        }
                            break;
                            
                        case 1: assert("RXP"==node->label());
                        {
                            assert(node->terminal);
                            const string content = node->lx->to_string();
                            std::cerr << "\t" << parent.label << "->" << content << std::endl;
                            parent << fetch_rxp(content);
                        }
                            break;
                            
                        case 2: assert("RAW"==node->label());
                        {
                            assert(node->terminal);
                            const string content = node->lx->to_string();
                            std::cerr << "\t" << parent.label << "->" << content << std::endl;
                            parent << fetch_raw(content);
                        }
                            break;
                            
                        case 3: assert("SUB"==node->label());
                        {
                            const string sub_label = parent.label + vformat("(%c%d)", rule::internal_char, ++(parent.prv));
                            aggregate   &sub = fetch_agg(sub_label);
                            grow(sub,node->ch->head);
                            parent << sub;
                        }
                            break;
                            
                        case 4: assert("ITEM"==node->label());
                        {
                            assert(2==node->ch->size);
                            const string  sub_label = parent.label + vformat("[%c%d]", rule::internal_char, ++(parent.prv));
                            aggregate    &sub = fetch_agg(sub_label);
                            sub.flags         = property::jettison;
                            const string  modifier = node->ch->tail->label();
                            delete node->ch->pop_back();
                            std::cerr << "\t\tmodifier=" << modifier << std::endl;
                            grow(sub,node->ch->head);
                            parent << __modified(sub, *xprs, modifier, hmod);
                        }
                            break;
                            
                        case 5: assert( "ALT"==node->label() );
                        {
                            alternate  &alt = xprs->alt();
                            grow(alt,node->ch->head);
                            parent << alt;
                        }
                            break;
                            
                            
                        default:
                            throw exception("unhandled grammar component '%s'", node->label().c_str());
                    }
                    node = node->next;
                }
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