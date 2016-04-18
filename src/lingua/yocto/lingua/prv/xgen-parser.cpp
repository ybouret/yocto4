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

            xgen:: Parser:: ~Parser() throw()
            {
            }

            xgen:: Parser:: Parser(const bool output_files) :
            lingua::parser("generator","main")
            {

                //______________________________________________________________
                //
                //
                // grammar description grammar !
                //
                //______________________________________________________________
                Agg &GRAMMAR = agg("XGEN");

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
                        Agg  &ITEM  = agg("ITEM");//,property::noSingle);
                        ITEM << ATOM;
                        {
                            Alt  &MOD  = alt();
                            MOD  << univocal('+') << univocal('*') << univocal('?');
                            ITEM << opt(MOD);
                        }

                        Rule &ITEMS = one_or_more(ITEM);
                        Agg  &ALT   = agg("ALT",property::standard);
                        ALT << ALTERN << ITEMS;

                        // if SUB exist => ALT !
                        Agg  &SUB  = agg("SUB");//,property::noSingle);
                        SUB << ITEMS << zero_or_more(ALT);

                        Agg &XPRN  = agg("XPRN");//,property::jettison);
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
                if(output_files)
                {
                    graphviz("xgen.dot");
                    ios::graphviz_render("xgen.dot");
                }

            }

        }

        parser * parser:: generate(ios::istream &fp, const bool output_files)
        {
            auto_ptr<syntax::xnode> tree;
            syntax::xgen::Parser    xparser(output_files);
            {

                //______________________________________________________________
                //
                // parse input to produce a raw tree
                //______________________________________________________________
                syntax::xnode *raw_tree = xparser.parse(fp);
                assert(raw_tree);
                tree.reset(raw_tree);
                if(output_files)
                {
                    raw_tree->graphviz("raw_tree.dot");
                    ios::graphviz_render("raw_tree.dot");
                }

                
                //______________________________________________________________
                //
                // rewrite the tree
                //______________________________________________________________
                //tree.forget();
                //tree.reset( syntax::xgen::rewrite(raw_tree) );

                if(output_files)
                {
                    tree->graphviz("usr_tree.dot");
                    ios::graphviz_render("usr_tree.dot");
                }
            }

            syntax::xgen xg;
            return xg.generate( &(*tree),output_files);
        }
        
    }
}
