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
            rules(),
            htop(YOCTO_MPERF_FOR(kw_top)),
            hsub(YOCTO_MPERF_FOR(kw_sub)),
            hmod(YOCTO_MPERF_FOR(kw_mod)),
            hres(YOCTO_MPERF_FOR(kw_res)),
            icom(0)
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
                //______________________________________________________________`
                xlist tmp;
                while(top_level.size>0)
                {
                    xnode *node = top_level.pop_front();
                    tmp.push_back(node);
                    switch(htop(node->label()))
                    {
                        case 0: assert("RULE"==node->label());
                            create_leading_rule(node);
                            break;
                            
                        case 1: assert("LXR"==node->label());
                            create_lexical_rule(node);
                            delete tmp.pop_back();
                            break;
                            
                        default:
                            throw exception("xgen.generate(unhandled top level '%s')", node->label().c_str());
                    }
                }
                top_level.swap_with(tmp);

                if(top_level.size<=0)
                {
                    throw exception("xgen.generate(NO SYNTAX RULE)");
                }

                //______________________________________________________________
                //
                // setting top_level code
                //______________________________________________________________`
                {
                    const xnode    *top_xnode = top_level.head;
                    const string    top_label = top_xnode->ch->head->lx->to_string();
                    rule_ptr       *ppTop     = rules.search(top_label);
                    if(!ppTop) throw exception("xgen.generate(unexpected no top-level '%s')", top_label.c_str());
                    xprs->top_level(**ppTop);
                }

                if(output_files)
                {
                    xprs->graphviz("usr_gram.dot");
                    ios::graphviz_render("usr_gram.dot");
                }


                xprs->check_consistency();
                
                return xprs.yield();
            }

            
            

        }
    }
}
