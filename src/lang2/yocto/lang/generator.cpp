#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/rstring.hpp"

#include "yocto/lang/syntax/langen.hxx"


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
            //Rule &CODE      = term("@","@",syntax::univocal);
            Rule &COLON     = term(":",":",syntax::jettison);
            Rule &SEMICOLON = term(";",";",syntax::jettison);

            Rule &LPAREN    = term("(","\\(",syntax::jettison);
            Rule &RPAREN    = term(")","\\)",syntax::jettison);
            Rule &ALTERNATE = term("|","\\|",syntax::jettison);


            //__________________________________________________________________
            //
            // Rule Definitions
            //__________________________________________________________________
            Agg  &RULE      = agg("RULE");

            RULE //<< opt(CODE)
            << ID << COLON;
            {
                Alt &ATOM = alt();
                ATOM << ID;
                ATOM << term<lexical::cstring>("RX");


                Agg  &ITEM  = agg("ITEM",syntax::mergeOne);
                ITEM << ATOM;
                Alt  &MOD  = alt();
                MOD << term("+","\\+",syntax::univocal);
                MOD << term("*","\\*",syntax::univocal);
                MOD << term("?","\\?",syntax::univocal);
                ITEM << opt(MOD);

                Rule &ITEMS = one_or_more(ITEM);
                Agg  &ALT   = agg("ALT");
                ALT << ALTERNATE << ITEMS;

                Agg  &SUB  = agg("SUB");
                SUB << ITEMS << zero_or_more(ALT);

                Agg &XPRN  = agg("XPRN", syntax::mergeOne);
                XPRN << LPAREN << SUB << RPAREN;

                ATOM << XPRN;

                RULE << SUB;
            }

            RULE << SEMICOLON;

            top_level( zero_or_more(RULE) );

            // some comments
            load<lexical::comment>("C++ Comment","//").hook(scanner);
            load<lexical::ccomment>("C Comment","/\\*","\\*/").hook(scanner);

            // other chars
            scanner.drop("WS", "[:blank:]");
            scanner.endl("ENDL");
        }

        syntax::xnode * generator::compile( ios::istream &fp )
        {
            syntax::xnode *tree = run(fp);
            reshape(tree);
            rewrite(tree);
            syntax::LanGen lg(tree);

            return tree;
        }

    }

}

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
                        ALT.push_back(node->pop_tail());
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





