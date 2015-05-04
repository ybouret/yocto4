#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"

#include "yocto/associative/set.hpp"
#include "yocto/ptr/intr.hpp"

#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/exception.hpp"
#include "yocto/lang/regexp.hpp"

#include <iostream>
namespace yocto
{
    namespace lang
    {

        namespace {

            class LangGen
            {
            public:
                typedef syntax::xnode         Node;
                typedef syntax::rule          Rule;
                typedef intr_ptr<string,Rule> pRule;
                typedef set<string,pRule>     RDict;

                const Node *root;
                RDict       rules;
                p_dict      terms;

                inline LangGen(const Node *n ) :
                root(n),
                rules(),
                terms()
                {
                    assert(root!=NULL);
                    collect(root);
                }

                inline ~LangGen() throw() {}


                void collect(const Node *node)
                {
                    if(node->terminal)
                    {
                        // a non terminal rule
                        if(node->label=="ID")
                        {
                            const string r_id = node->lex().to_string();
                            if( !rules.search(r_id) )
                            {
                                std::cerr << "New ID: " << node->lex() << std::endl;
                                const pRule pR( new syntax::aggregate(r_id,syntax::standard) );
                                if(!rules.insert(pR))
                                {
                                    throw exception("Failure in inserting '%s'", r_id.c_str());
                                }
                            }
                            return;
                        }


                        if(node->label=="RX")
                        {
                            const string p_id = node->lex().to_string();
                            if( !terms.search(p_id) )
                            {
                                std::cerr << "New RX: " << node->lex() << std::endl;
                                terms.define(p_id, regexp(p_id, &terms) );
                            }
                            return;
                        }

                    }
                    else
                    {
                        for(const Node *child = node->children().head; child;child=child->next)
                        {
                            collect(child);
                        }
                    }

                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(LangGen);
            };
        }

    }

}


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
            Rule &CODE      = term("@","@",syntax::univocal);
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

            RULE << opt(CODE) << ID << COLON;
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
            LangGen lg(tree);

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
                    syntax::xnode *child = node->pop();
                    reshape(child);
                    if(node->label=="SUB" && child->label=="SUB")
                    {
                        while(child->children().size)
                        {
                            stk.push_back(child->pop());
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





