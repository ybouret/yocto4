#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"
#include <iostream>
#include <cstdlib>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            xgen:: ~xgen() throw()
            {
            }


            static const char *main_keywords[] =
            {
                "RULE", //0
                "LXR" , //1
                "PTY"   //2
            };

            static const char *code_keywords[] =
            {
                "@drop",      // 0
                "@endl",      // 1
                "@comment"    // 2
            };

            static const char *meta_keywords[] =
            {
                "RXP", //0
                "RAW"  //1
            };

            static const char *kind_keywords[] =
            {
                "?", //0
                "*", //1
                "+"  //2
            };


            static const char *grow_keywords[] =
            {
                "SUB",
                "ALT",
                "RAW",
                "RXP",
                "ID",
                "ITEM"
            };

            xgen:: xgen(const xnode *node) :
            root(node),
            xprs(0),
            name(0),
            agg(),
            rxp(),
            raw(),
            zmph( YOCTO_MPERF_FOR(main_keywords) ),
            cmph( YOCTO_MPERF_FOR(code_keywords) ),
            mmph( YOCTO_MPERF_FOR(meta_keywords) ),
            kmph( YOCTO_MPERF_FOR(kind_keywords) ),
            gmph( YOCTO_MPERF_FOR(grow_keywords) ),
            indx(0),
            jndx(0),
            visited(),
            curr(0),
            no_single(),
            one_level()
            {
                assert(root);
                assert(!root->terminal);
                assert("parser"==root->label);

                //______________________________________________________________
                //
                // Find the parser name and create it
                //______________________________________________________________
                assert(root->size()>0);
                const xnode         *ch          = root->head();     assert(ch); assert("NAME"==ch->label);
                const string         parser_name = ch->content(1,0);
                const string         parser_main = "main";
                xprs.reset( new parser(parser_name,parser_main) );

                name = xprs->grammar::name.c_str();

                //______________________________________________________________
                //
                // First Pass: Study Top Level rules
                //______________________________________________________________
                for(ch=ch->next;ch;ch=ch->next)
                {
                    switch(zmph(ch->label))
                    {
                        case 0: assert("RULE"==ch->label); process_rule_level1(ch); break;
                        case 1: assert("LXR" ==ch->label); process_lxr__level1(ch); break;
                        case 2: assert("PTY" ==ch->label); process_pty__level1(ch); break;
                        default: assert(-1==zmph(ch->label));
                            throw exception("%s: unexpected top level '%s'", name, ch->label.c_str());
                    }
                }

                check_properties();

                //______________________________________________________________
                //
                // Second Pass: Processing Top Level Rules
                //______________________________________________________________
                ch=root->head();
                for(ch=ch->next;ch;ch=ch->next)
                {
                    if("RULE"!=ch->label) continue;
                    process_rule_level2(ch);
                }

                //______________________________________________________________
                //
                // Third Pass: Check Connectivity
                //______________________________________________________________
                visited.ensure( xprs->count() );
                check_connectivity( &xprs->top_level() );
                check_connected(agg);
                check_connected(rxp);
                check_connected(raw);
                xprs->cleanup();

                //______________________________________________________________
                //
                // Fourth Pass: Check Semantic
                //______________________________________________________________
                visited.free();
                check_semantic( &xprs->top_level() );



                //xprs->gramviz("lanraw.dot");
                //(void)system("dot -Tpng -o lanraw.png lanraw.dot");
                
            }
            
            logical * xgen:: create_sub()
            {
                assert(curr!=NULL);
                const string label = curr->label;
                const string id    = label + vformat("%c%u", grammar::internal_char, ++indx);
                return & xprs->agg(id, one_level.search(label) ? mergeAll : mergeOne);
            }
            
            void xgen:: delete_sub(const string &label)
            {
                xprs->remove(label);
                --indx;
            }
            
            
        }
        
    }
}
