#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/rstring.hpp"
#include "yocto/lang/xgen/xgen.hxx"

#include "yocto/exception.hpp"
#include <cstdlib>


namespace yocto
{
    namespace lang
    {

        generator:: ~generator() throw()
        {
        }

        generator:: generator(const char *langID,const bool output_files) :
        parser(langID,"main"),
        output(output_files)
        {

            //__________________________________________________________________
            //
            // Terminals
            //__________________________________________________________________
            Rule &ID        = term("ID", "[:word:]+");
            Rule &COLON     = text(":",syntax::jettison);
            Rule &SEMICOLON = text(";",syntax::jettison);

            Rule &LPAREN    = term("(","\\(",syntax::jettison);
            Rule &RPAREN    = term(")","\\)",syntax::jettison);
            Rule &ALTERN    = term("|","\\|",syntax::jettison);
            Rule &RXP       = term<lexical::cstring>("RXP");
            Rule &RAW       = term<lexical::rstring>("RAW");


            //__________________________________________________________________
            //
            // name definition
            //__________________________________________________________________
            Agg &USR_NAME = agg("USR",syntax::mergeOne);
            USR_NAME << term("NAME","\\.[:word:]+");
            USR_NAME << SEMICOLON;

            //__________________________________________________________________
            //
            // Rule Definitions
            //__________________________________________________________________
            Agg  &RULE      = agg("RULE");

            RULE << ID;

            RULE << COLON;
            {
                Alt &ATOM = alt();
                ATOM << ID;
                ATOM << RXP;
                ATOM << RAW;

                Agg  &ITEM  = agg("ITEM",syntax::mergeOne);
                ITEM << ATOM;
                Alt  &MOD  = alt();
                MOD  << term("+","\\+",syntax::univocal);
                MOD  << term("*","\\*",syntax::univocal);
                MOD  << term("?","\\?",syntax::univocal);
                ITEM << opt(MOD);

                Rule &ITEMS = one_or_more(ITEM);
                Agg  &ALT   = agg("ALT");
                ALT << ALTERN << ITEMS;

                Agg  &SUB  = agg("SUB");
                SUB << ITEMS << zero_or_more(ALT);

                Agg &XPRN  = agg("XPRN", syntax::mergeOne);
                XPRN << LPAREN << SUB << RPAREN;

                ATOM << XPRN;

                RULE << SUB;
            }

            RULE << SEMICOLON;

            //__________________________________________________________________
            //
            // LEXICAL RULE
            //__________________________________________________________________
            Agg &LXR = agg("LXR");
            LXR << term("LX","@[:word:]+");
            LXR << COLON;
            LXR << one_or_more(choice(RXP,RAW));
            LXR << SEMICOLON;


            //__________________________________________________________________
            //
            // PROPERTY RULE
            //__________________________________________________________________

            Agg &PTY = agg("PTY");
            PTY << term("PN","$[:word:]+");
            PTY << COLON;
            PTY << zero_or_more(ID);
            PTY << SEMICOLON;

            //__________________________________________________________________
            //
            // And finally assemble the parser...
            //__________________________________________________________________
            Agg &THE_PARSER = agg("parser");
            THE_PARSER << USR_NAME;
            THE_PARSER << zero_or_more(choice(RULE,LXR,PTY));


            top_level( THE_PARSER );

            //__________________________________________________________________
            //
            // some comments
            //__________________________________________________________________
            load<lexical::comment>("C++ Comment","//").hook(scanner);
            load<lexical::ccomment>("C Comment","/\\*","\\*/").hook(scanner);

            //__________________________________________________________________
            //
            // other chars
            //__________________________________________________________________
            scanner.drop("WS", "[:blank:]");
            scanner.endl("ENDL");

#if 0
            //__________________________________________________________________
            //
            // saving the generator grammar
            //__________________________________________________________________
            if(output)
            {
                grammar::gramviz("ygen_gram.dot");
                (void)system("dot -Tpng -o ygen_gram.png ygen_gram.dot");
            }
#endif

        }

    }

}

#include "yocto/ios/graphviz.hpp"

namespace yocto
{

    namespace lang
    {


        parser *generator::compile( ios::istream &fp )
        {
            //__________________________________________________________________
            //
            // Parse the input
            //__________________________________________________________________
            syntax::xnode          *tree = run(fp);
            if(!tree)
            {
                throw exception("parser::generator: unexpected empty source tree!");
            }
            auto_ptr<syntax::xnode> guard(tree);
            if(output)
            {
                tree->graphviz("lang_raw.dot");
                ios::graphviz_render("lang_raw.dot");
                //(void) system("dot -Tpng -o lang_raw.png lang_raw.dot");
            }

            //__________________________________________________________________
            //
            // Rewrite for simplification
            //__________________________________________________________________
            rewrite( tree );
            if(output)
            {
                tree->graphviz("lang_tree.dot");
                ios::graphviz_render("lang_tree.dot");
                //(void) system("dot -Tpng -o lang_tree.png lang_tree.dot");
            }

            //__________________________________________________________________
            //
            // generate the new parser/grammar
            //__________________________________________________________________
            syntax::xgen   xg(tree);
            xg.xprs->lock();
            if(output)
            {
                xg.xprs->gramviz("lang_gram.dot");
                ios::graphviz_render("lang_gram.dot");
                //(void) system("dot -Tpng -o lang_gram.png lang_gram.dot");
            }
            return xg.xprs.yield();
        }

    }

}

namespace yocto
{
    namespace lang
    {

        void generator:: rewrite( syntax::xnode *node )
        {
            if(!node->terminal)
            {
                for( syntax::xnode *child=node->children().head;child;child=child->next)
                {
                    rewrite(child);
                }

                if(node->label=="SUB")
                {
                    syntax::xnode::leaves  ALT;
                    while(node->children().size&&node->children().tail->label=="ALT")
                    {
                        ALT.push_front(node->pop_tail());
                    }

                    if(ALT.size>0)
                    {
                        assert(node->children().size>0);

                        syntax::xnode *xalt = syntax::xnode::leaf(get_rule("ALT"),syntax::standard);
                        while(node->children().size)
                        {
                            xalt->append( node->pop_head() );
                        }
                        node->append(xalt);
                        
                        while(ALT.size)
                        {
                            node->append(ALT.pop_front());
                        }
                        
                    }
                    
                }
            }
        }
        
    }
    
}





