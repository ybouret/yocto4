#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/ptr/auto.hpp"

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


            static const char *kw_db[] =
            {
                "ID",
                "RXP",
                "RAW"
            };

            xgen:: xgen() :
            xprs(NULL),
            adb(),
            h_db(YOCTO_MPERF_FOR(kw_db))
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
                    const xnode *node      = top_level.head;
                    assert("NAME"==node->label()); assert(node->terminal);
                    const string langID    = node->lx->to_string(1,0);
                    const string rootID    = "main";
                    xprs.reset( new lingua::parser(langID,rootID) );
                    delete top_level.pop_front();
                }
                Y_XGEN_OUT(std::cerr << "parser created with " << xprs->grammar::name << "," << xprs->lexer::name << std::endl);

                //______________________________________________________________
                //
                // pass1: build databases for syntax
                //______________________________________________________________
                for(const xnode *node = top_level.head; node; node=node->next)
                {
                    if("RULE"==node->label())
                    {
                        build_syntax_databases(node);
                    }
                }


                return xprs.yield();
            }

        }

        
    }
}
