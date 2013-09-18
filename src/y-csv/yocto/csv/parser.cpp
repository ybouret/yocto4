#include "yocto/csv/parser.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/ptr/auto.hpp"

//#define Y_CSV_OUTPUT
#if defined(Y_CSV_OUTPUT)
#include <iostream>
#include "yocto/ios/ocstream.hpp"
#endif

#include "yocto/rx/pattern/logic.hpp"

namespace yocto
{
    namespace CSV
    {
        using namespace lang;
        
        
        class Parser:: Impl : public object, public parser
        {
        public:
            explicit Impl() :
            parser( "CSV:Lexer", "CSV::Parser" )
            {
                
                //==============================================================
                //
                // declare CSV terminal
                //
                //==============================================================
                syntax::terminal &STRING = terminal("STRING", "[:cstring:]");
                syntax::terminal &COMA   = terminal("COMA",',', syntax::is_discardable);
                syntax::terminal &ID     = terminal("ID","[-+.[:word:]]+");
                syntax::terminal &ENDL   = term("ENDL",syntax::is_discardable);
                //syntax::terminal &EMPTY  = term("EMPTY");
                scan.make( "BLANKS", "[ \t]+",   & scan.discard );
                scan.make( "ENDL",   "[:endl:]", & scan.newline);
                //scan.make( "EMPTY", regex::logical::AND::create() );
                //==============================================================
                //
                // declare CSV grammar
                //
                //==============================================================
                
                //--------------------------------------------------------------
                // ITEM : STRING;
                //--------------------------------------------------------------
                syntax::alternate & ITEM = alt("ITEM");
                ITEM |= STRING;
                ITEM |= ID;
                //ITEM |= EMPTY;
                
                //--------------------------------------------------------------
                // OTHER : COMA ITEM ;
                //--------------------------------------------------------------
                syntax::aggregate & OTHER = agg("OTHER", syntax::is_merging_all);
                OTHER << COMA << ITEM;
                
                //--------------------------------------------------------------
                // LINE : ITEM OTHER* ENDL;
                //--------------------------------------------------------------
                syntax::aggregate &LINE  = agg("LINE");
                LINE &= ITEM;
                LINE &= rep("OTHERS", OTHER, 0);
                LINE &= ENDL;
                
                //--------------------------------------------------------------
                // INSTANCE : LINE*;
                //--------------------------------------------------------------
                syntax::repeating &INSTANCE = rep("INSTANCE", LINE, 0);
                
                
                set_root(INSTANCE);                
            }
            
            virtual ~Impl() throw()
            {
            }
            
            
            void call( Lines &lines, ios::istream &fp )
            {
                lines.free();
                
                //-- reset lexer
                reset();
                
                //-- create source from fp
                regex::source                Src(fp);
                
                //-- build the parse tree
                auto_ptr<syntax::parse_node> Tree( accept(*this,Src) );
                
                //-- simplify the tree
                Tree->AST();

#if defined (Y_CSV_OUTPUT)
                std::cerr << "Saving tree..." << std::endl;
                {
                    ios::ocstream os("csv.dot",false);
                    Tree->graphviz("G", os);
                }
                system( "dot -Tpng csv.dot -o csv.png" );
#endif
                
                //--------------------------------------------------------------
                // Load the tree
                //--------------------------------------------------------------
                syntax::parse_node::child_list &rows = Tree->children();
                for( syntax::parse_node *node = rows.head; node; node=node->next)
                {
                    assert(node->label=="LINE");
                    {
                        Line empty;
                        lines.push_back(empty);
                    }
                    Line &line = lines.back();
                    for( syntax::parse_node *item = node->children().head; item; item=item->next)
                    {
                        const lexeme *lx = item->lex();
                        if( item->label == "STRING" )
                        {
                            const string tmp = lx->to_string(1,1);
                            line.push_back(tmp);
                            continue;
                        }
                        
                        {
                            const string tmp = lx->to_string();
                            line.push_back(tmp);
                        }
                        
                    }
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Impl);
        };
        
        
        Parser:: Parser() : impl( new Impl() )
        {
        }
        
        
        Parser:: ~Parser() throw()
        {
            delete impl;
        }
        
        void Parser:: operator()( Lines &lines, ios::istream &fp )
        {
            impl->call(lines,fp);
        }
        
    }
}

