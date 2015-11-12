#include "yocto/lang/generator.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/rstring.hpp"
#include "yocto/lang/xgen/xgen.hxx"

#include "yocto/exception.hpp"
#include "yocto/ios/graphviz.hpp"

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
            Rule &COLON     = text(':',syntax::jettison);
            Rule &SEMICOLON = text(';',syntax::jettison);

            Rule &LPAREN    = text('(',syntax::jettison);
            Rule &RPAREN    = text(')',syntax::jettison);
            Rule &ALTERN    = text('|',syntax::jettison);
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

            RULE << ID << COLON;
            {
                Alt &ATOM = alt();
                ATOM << ID;
                ATOM << RXP;
                ATOM << RAW;

                Agg  &ITEM  = agg("ITEM",syntax::mergeOne);
                ITEM << ATOM;
                Alt  &MOD  = alt();
                MOD  << text('+',syntax::univocal);
                MOD  << text('*',syntax::univocal);
                MOD  << text('?',syntax::univocal);
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
                //std::cerr << "Saving YGEN_GRAM" << std::endl;
                grammar::gramviz("ygen_gram.dot");
                ios::gramviz("ygen_gram.dot");
            }
#endif

        }

    }

}



#include "yocto/ios/ocstream.hpp"
#include <iostream>
namespace yocto
{

    namespace lang
    {



        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////

        namespace
        {
            class grammar_emitter
            {
            public:
                ios::ostream &fp;

                explicit grammar_emitter(const syntax::xnode *tree, ios::ostream &tgt ) :
                fp(tgt)
                {
                    emit_tree(tree);
                }


                inline void emit_tree(const syntax::xnode *tree )
                {
                    assert(tree);
                    assert("parser"==tree->label);
                    assert(tree->size()>0);
                    const syntax::xnode *r    = tree->head(); assert(r); assert("NAME"==r->label);
                    const string         name = r->content();
                    fp << name << ";\n";
                    for(r=r->next;r;r=r->next)
                    {
                        emit_new(r);
                    }
                }

                inline void emit_new(const syntax::xnode *r )
                {
                    assert(r);
                    const string &label = r->label;
                    std::cerr << "Emit new " << label << std::endl;

                    if(label=="RULE")
                    {
                        emit_rule(r);
                        return;
                    }

                    if(label=="LXR")
                    {

                        return;
                    }

                    if(label=="PTY")
                    {

                        return;
                    }

                    throw exception("unhandled '%s'", label.c_str());

                }

                inline void emit_rule(const syntax::xnode *r)
                {
                    assert("RULE"==r->label);
                    assert(r->size()>0);
                    const syntax::xnode *curr = r->head();
                    assert("ID"==curr->label);
                    const string ID = curr->content();
                    fp << ID << ':';


                    fp << ';' << '\n';
                }


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(grammar_emitter);
            };
        }




        ////////////////////////////////////////////////////////////////////////
        //
        // Compile from grammar
        //
        ////////////////////////////////////////////////////////////////////////
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
                tree->graphviz("usr_def.dot");
                ios::graphviz_render("usr_def.dot");
            }

            //__________________________________________________________________
            //
            // Rewrite for simplification
            //__________________________________________________________________
            rewrite( tree );
            if(output)
            {
                tree->graphviz("usr_tree.dot");
                ios::graphviz_render("usr_tree.dot");
            }

            //__________________________________________________________________
            //
            // This tree is printable
            //__________________________________________________________________
            {
                ios::wcstream fp2( "tmp.g" );
                {
                    grammar_emitter emit(tree,fp2);
                }
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





