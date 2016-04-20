#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/exception.hpp"

#define Y_XGEN_VERBOSE 1

#if Y_XGEN_VERBOSE == 1
#include <iostream>
#define Y_XGEN_OUT(CODE) do { std::cerr << "[XGEN] "; CODE; } while(false)
#else
#define Y_XGEN_OUT(CODE)
#endif


namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            xgen:: ~xgen() throw()
            {
            }


            static const char *kw_top[] =
            {
                "RULE",
                "LXR"
            };
            
            static const char *kw_sub[] =
            {
                "ID",
                "RXP",
                "RAW",
                "SUB",
                "ITEM",
                "ALT"
            };

            static const char *kw_mod[] =
            {
                "?",
                "*",
                "+"
            };

            static const char *kw_res[] =
            {
                "drop",
                "endl",
                "comment"
            };
            
            xgen:: xgen() :
            xprs(NULL),
            agg_db(),
            rxp_db(),
            raw_db(),
            htop(YOCTO_MPERF_FOR(kw_top)),
            hsub(YOCTO_MPERF_FOR(kw_sub)),
            hmod(YOCTO_MPERF_FOR(kw_mod)),
            hres(YOCTO_MPERF_FOR(kw_res))
            {
            }


            lingua::parser *xgen:: generate(xnode *tree, const bool output_files)
            {

                assert(tree);
                assert(aggregate::UUID==tree->origin->uuid);
                assert("XGEN"==tree->label());
                assert(false ==tree->terminal);

                //______________________________________________________________
                //
                // using top level list
                //______________________________________________________________
                xlist &top_level = *(tree->ch);    assert(top_level.size>0);

                //______________________________________________________________
                //
                // create the parser with the name
                //______________________________________________________________
                {
                    const xnode *node      = top_level.head;  assert("NAME"==node->label()); assert(node->terminal);
                    const string langID    = node->lx->to_string(1,0);
                    const string rootID    = "main";

                    xprs.reset( new lingua::parser(langID,rootID) );

                    delete top_level.pop_front();
                }
                Y_XGEN_OUT(std::cerr << "parser created with " << xprs->grammar::name << "," << xprs->lexer::name << std::endl);

                //______________________________________________________________
                //
                // plugin detection
                //______________________________________________________________
                initialize(top_level);
                
                //______________________________________________________________
                //
                // run over top level rule
                //______________________________________________________________
                for(const xnode *node = top_level.head; node; node=node->next)
                {
                    switch(htop(node->label()))
                    {
                        case 0: assert("RULE"==node->label());
                            create_rule(node);
                            break;
                            
                        case 1: assert("LXR"==node->label());
                            create_lxr_(node);
                            break;
                            
                        default:
                            throw exception("xgen.generate: unhandled top level '%s'", node->label().c_str());
                    }
                
                }

                if(output_files)
                {
                    xprs->graphviz("usr_gram.dot");
                    ios::graphviz_render("usr_gram.dot");
                }

                //______________________________________________________________
                //
                // Enforce top level rule
                //______________________________________________________________
                if(agg_db.size()>0)
                {
                    aggDB::iterator first = agg_db.begin();
                    xprs->top_level(**first);
                }
                xprs->check_consistency();
                
                return xprs.yield();
            }

            
            void xgen:: initialize( xlist &top_level )
            {
                xlist tmp;
                while(top_level.size)
                {
                    xnode *node = top_level.pop_front();
                    tmp.push_back(node);
                    if(1==htop(node->label()))
                    {
                        assert("LXR"==node->label());
                        const string lx_id = node->ch->head->lx->to_string(1,0);
                        std::cerr << "lexical rule id=" << lx_id << std::endl;
                        if(hres(lx_id)>=0)
                        {
                            //-- this is a reserved keyword, will be processed later
                            continue;
                        }
                        else
                        {
                            //-- this must be a plugin
                            std::cerr << "\tPLUGIN" << std::endl;
                            delete tmp.pop_back();
                        }
                    }
                }
                top_level.swap_with(tmp);
            }

            void xgen::load_plugin(const string &id)
            {
                
            }


        }
    }
}
