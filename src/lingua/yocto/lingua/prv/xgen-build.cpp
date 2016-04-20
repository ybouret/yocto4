#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/exception.hpp"
#include "yocto/lingua/lexical/plugin/_string.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            rule       &xgen::fetch_agg(const string &label)
            {
                rule_ptr *ppRule = rules.search(label);
                if(ppRule)
                {
                    // may be a terminal plugged in
                    std::cerr << "returning '" << label << "'" << std::endl;
                    return **ppRule;
                }
                else
                {
                    aggregate      &r = xprs->agg(label);
                    const rule_ptr p(&r);
                    if(!rules.insert(p))
                    {
                        throw exception("unexpected multiple ID '%s'", label.c_str());
                    }
                    std::cerr << "creating  '" << label << "'" << std::endl;
                    return r;

                }
            }

            rule & xgen::fetch_rxp(const string &label)
            {
                rule_ptr *ppRxp = rules.search(label);
                if(ppRxp)
                {
                    std::cerr << "returning rxp '" << label << "'" << std::endl;
                    return **ppRxp;
                }
                else
                {
                    const string expr = strconv::to_cstring(label);
                    rule &r = xprs->terminal(label, expr);

                    const rule_ptr tmp(&r);
                    if(!rules.insert(tmp))
                    {
                        throw exception("unexpected multiple RXP '%s'", label.c_str());

                    }
                    std::cerr << "creating  rxp '" << expr << "'" << std::endl;
                    return r;
                }
            }

            rule & xgen::fetch_raw(const string &label)
            {
                rule_ptr *ppRxp = rules.search(label);
                if(ppRxp)
                {
                    std::cerr << "returning raw '" << label << "'" << std::endl;
                    return **ppRxp;
                }
                else
                {
                    rule &r = xprs->univocal(label);

                    const rule_ptr tmp(&r);
                    if(!rules.insert(tmp))
                    {
                        throw exception("unexpected multiple RAW '%s'", label.c_str());
                    }
                    std::cerr << "creating  raw '" << label << "'" << std::endl;
                    return r;
                }
            }



            void xgen::create_leading_rule( const xnode *top )
            {
                assert("RULE"==top->label());
                assert(false==top->terminal);
                const xnode *node = top->ch->head;

                //-- get the rule label from "ID"
                assert("ID"==node->label());
                assert(true==node->terminal);
                const string id = node->lx->to_string();
                rule        &r  = fetch_agg(id);
                if(r.uuid != aggregate::UUID)
                {
                    throw exception("xgen: top-level '%s' is not an aggregate!", id.c_str());
                }
                grow(r.as<aggregate>(),node->next);

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
                            aggregate   &sub = fetch_agg(sub_label).as<aggregate>();
                            grow(sub,node->ch->head);
                            parent << sub;
                        }
                            break;

                        case 4: assert("ITEM"==node->label());
                        {
                            assert(2==node->ch->size);
                            const string  sub_label = parent.label + vformat("[%c%d]", rule::internal_char, ++(parent.prv));
                            aggregate    &sub       = fetch_agg(sub_label).as<aggregate>();
                            const string  modifier  = node->ch->tail->label();
                            sub.flags               = property::jettison;

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

#include "yocto/lingua/lexical/plugin/_string.hpp"
#include "yocto/lingua/lexical/plugin/end_of_line_comment.hpp"
#include "yocto/lingua/lexical/plugin/block-comment.hpp"
#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {


            static inline
            pattern *string2pattern(const string &label,
                                    const string &expr,
                                    p_dict       &dict)
            {
                if("RAW"==label)
                {
                    return logical::equal(expr);
                }

                if("RXP"==label)
                {
                    return regexp(expr,&dict);
                }

                throw exception("unexpected '%s' in lexical rule", label.c_str());
            }


            static inline
            pattern *xnode2pattern(const xnode *args, p_dict &dict)
            {
                assert(args);
                assert(args->terminal);

                const string &kind    = args->origin->label; assert("RAW"==kind||"RXP"==kind);
                const string content  = args->lx->to_string();
                const string expr     = strconv::to_cstring(content);
                //std::cerr << "content='" << content << "'=>'" << expr << "'" << std::endl;

                auto_ptr<pattern> ans( string2pattern(kind,expr,dict) );
                if(ans->match_empty())
                {
                    throw exception("%d: %s '%s' matches empty", args->lx->line, kind.c_str(), content.c_str());
                }

                return ans.yield();
            }

            static inline
            pattern *create_from(const xnode *args, p_dict &dict)
            {
                assert(args);
                if(args->next)
                {
                    auto_ptr<logical> q( OR::create() );
                    while( args )
                    {
                        q->add(xnode2pattern(args,dict));
                        args = args->next;
                    }
                    return q.yield();
                }
                else
                {
                    return xnode2pattern(args,dict);
                }
            }


            static inline
            void make_eol_comment(int &icom, parser &prs,const xnode *args)
            {
                assert(args);
                assert(args->terminal);

                const string label = vformat("comment%d",++icom);
                const string content = args->lx->to_string();
                const string expr    = strconv::to_cstring(content);
                prs.load<lexical::end_of_line_comment>(label,expr).hook(prs.root);
                

            }

            static inline
            void make_blk_comment(int &icom, parser &prs, const xnode *args)
            {
                assert(args);
                assert(args->terminal);
                assert(args->next);
                assert(args->next->terminal);

                const string label = vformat("comment%d",++icom);
                const string callContent = args->lx->to_string();
                const string callExpr    = strconv::to_cstring(callContent);
                const string backContent = args->next->lx->to_string();
                const string backExpr    = strconv::to_cstring(backContent);
                prs.load<lexical::block_comment>(label,callExpr,backExpr).hook(prs.root);
            }



            void xgen::create_lexical_rule( const xnode *top )
            {
                assert("LXR"==top->label());
                assert(false==top->terminal);
                const xlist &info = *(top->ch);
                const xnode *node = info.head;

                assert("LX"==node->label());
                assert(true==node->terminal);

                const string id   = node->lx->to_string(1,0);
                const xnode *args = node->next;

                switch(hres(id))
                {
                    case 0:
                    {
                        const lexical::action __drop( & (xprs->root), & lexical::scanner::discard );
                        xprs->root.make(id, create_from(args,xprs->dict), __drop);
                    } break;

                    case 1:
                    {
                        const lexical::action __endl( & (xprs->root), &lexical::scanner::newline );
                        xprs->root.make(id, create_from(args,xprs->dict), __endl);
                    }  break;
                        
                    case 2:
                    {
                        switch(info.size)
                        {
                            case 2:
                                make_eol_comment(icom,*xprs,args);
                                break;

                            case 3:
                                make_blk_comment(icom,*xprs,args);
                                break;

                            default:
                                throw exception("comment must be followed by 1 (=>eol) or 2(=>blk) strings");
                        }
                    } break;
                        
                    default:
                        throw exception("xgen: unhandled lexical rule '%s'", id.c_str());
                }
            }
            
        }
        
    }
    
}