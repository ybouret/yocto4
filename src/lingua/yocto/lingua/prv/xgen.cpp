#include "yocto/lingua/prv/xgen.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/graphviz.hpp"
#include "yocto/lingua/lexical/plugin/cstring.hpp"
#include "yocto/lingua/lexical/plugin/rstring.hpp"
#include "yocto/lingua/lexical/plugin/end_of_line_comment.hpp"
#include "yocto/lingua/lexical/plugin/block-comment.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            xgen:: ~xgen() throw()
            {
            }

            xgen:: xgen(const bool output_files) :
            lingua::parser("generator","main"),
            output(output_files)
            {

                //______________________________________________________________
                //
                //
                // grammar description grammar !
                //
                //______________________________________________________________
                Agg &GRAMMAR = agg("GRAMMAR");

                dict.define("ID", "[[:alpha:]_][:word:]*");

                Rule &END     = jettison(';');

                //______________________________________________________________
                //
                // fist item: the lang name...
                //______________________________________________________________
                {
                    Agg &USR_NAME = agg("USR",property::jettison);
                    USR_NAME << terminal("NAME","\\.{ID}");
                    USR_NAME << END;
                    GRAMMAR  << USR_NAME;
                }

                //______________________________________________________________
                //
                // Common terminals
                //______________________________________________________________
                Rule &ID        = terminal("ID","{ID}");
                Rule &COLON     = jettison(':');
                Rule &RXP       = term<lexical::cstring>("RXP");
                Rule &RAW       = term<lexical::rstring>("RAW");
                Rule &ALTERN    = jettison('|');
                Rule &LPAREN    = jettison('(');
                Rule &RPAREN    = jettison(')');

                //______________________________________________________________
                //
                // Rule Definitions
                //______________________________________________________________

                Agg  &RULE      = agg("RULE");
                {

                    RULE << ID << COLON;

                    {
                        Alt &ATOM = alt();
                        ATOM << ID;
                        ATOM << RXP;
                        ATOM << RAW;
                        Agg  &ITEM  = agg("ITEM",property::noSingle);
                        ITEM << ATOM;
                        {
                            Alt  &MOD  = alt();
                            MOD  << univocal('+') << univocal('*') << univocal('?');
                            ITEM << opt(MOD);
                        }

                        Rule &ITEMS = one_or_more(ITEM);
                        Agg  &ALT   = agg("ALT",property::standard);
                        ALT << ALTERN << ITEMS;

                        Agg  &SUB  = agg("SUB",property::noSingle);
                        SUB << ITEMS << zero_or_more(ALT);

                        Agg &XPRN  = agg("XPRN",property::jettison);
                        XPRN << LPAREN << SUB << RPAREN;

                        ATOM << XPRN;

                        RULE << SUB;



                    }

                    RULE << END;
                }


                //______________________________________________________________
                //
                // LEXICAL RULE DEFINITION
                //______________________________________________________________
                Agg &LXR = agg("LXR");
                LXR << terminal("LX","@{ID}");
                LXR << COLON;
                LXR << one_or_more(choice(RXP,RAW));
                LXR << END;


                //______________________________________________________________
                //
                // assemble parser grammar
                //______________________________________________________________
                GRAMMAR << zero_or_more(choice(RULE,LXR));

                //__________________________________________________________________
                //
                // some comments
                //__________________________________________________________________
                load<lexical::end_of_line_comment> ("C++ Comment","//").hook(root);
                load<lexical::block_comment>("C Comment",  "/\\*","\\*/").hook(root);


                //______________________________________________________________
                //
                // extra lexical rules
                //______________________________________________________________
                root.drop("WS", "[:blank:]");
                root.endl("ENDL");


                //______________________________________________________________
                //
                // done
                //______________________________________________________________
                if(output)
                {
                    graphviz("xgen.dot");
                    ios::graphviz_render("xgen.dot");
                }
                
            }
            
        }
        
        parser * parser:: generate(ios::istream &fp, const bool output_files)
        {
            syntax::xgen   generator(output_files);
            syntax::xnode *tree = generator.parse(fp);
            assert(tree);
            auto_ptr<syntax::xnode> guard(tree);
            if(output_files)
            {
                tree->graphviz("usr_tree.dot");
                ios::graphviz_render("usr_tree.dot");
            }
            return 0;
        }
        
    }
}
