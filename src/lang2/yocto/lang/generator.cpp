#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/rstring.hpp"

#include "yocto/lang/syntax/langen.hxx"

#include <cstdlib>


namespace yocto
{
    namespace lang
    {

        generator:: ~generator() throw()
        {
        }

        generator:: generator(const char *langID) :
        parser(langID,"main")
        {

            //__________________________________________________________________
            //
            // Terminals
            //__________________________________________________________________

            Rule &ID        = term("ID", "[:word:]+");
            Rule &COLON     = term(":",":",syntax::jettison);
            Rule &SEMICOLON = term(";",";",syntax::jettison);

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
            LXR << choice(RXP,RAW);
            LXR << SEMICOLON;


            Agg &THE_PARSER = agg("parser");
            THE_PARSER << USR_NAME;
            THE_PARSER << zero_or_more(choice(RULE,LXR));


            top_level( THE_PARSER );


            // some comments
            load<lexical::comment>("C++ Comment","//").hook(scanner);
            load<lexical::ccomment>("C Comment","/\\*","\\*/").hook(scanner);

            // other chars
            scanner.drop("WS", "[:blank:]");
            scanner.endl("ENDL");

            grammar::gramviz("ggram.dot");
            (void)system("dot -Tpng -o ggram.png ggram.dot");

        }

        parser *generator::compile( ios::istream &fp )
        {
            syntax::xnode          *tree = run(fp);
            auto_ptr<syntax::xnode> guard(tree);
            rewrite( tree );
            if(tree)
            {
                tree->graphivz("xnode.dot");
                (void) system("dot -Tpng -o xnode.png xnode.dot");
            }

            syntax::LanGen lg(tree);

            return lg.P.yield();
        }

    }

}

#if 0
namespace yocto
{
    namespace lang
    {

        void generator::reshape( syntax::xnode *node ) throw()
        {
            if(!node->terminal)
            {
                syntax::xnode::leaves stk;
                while(node->children().size)
                {
                    syntax::xnode *child = node->pop_head();
                    reshape(child);
                    if(node->label=="SUB" && child->label=="SUB")
                    {
                        while(child->children().size)
                        {
                            stk.push_back(child->pop_head());
                        }
                        delete child;
                    }
                    else
                    {
                        stk.push_back(child);
                    }
                }
                while(stk.size)
                {
                    node->append(stk.pop_front());
                }
            }
        }


    }

}
#endif



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





