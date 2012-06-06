#include "yocto/utest/run.hpp"
#include "yocto/lang/grammar.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/ios/ocstream.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(parse)
{
    std::cerr << "sizeof(lexeme)                = " << sizeof(lexeme) << std::endl;
    std::cerr << "sizeof(syntax::parse_node)    = " << sizeof(syntax::parse_node) << std::endl;
    std::cerr << "syntax::parse_node::data_size = " << syntax::parse_node::data_size << std::endl;
    std::cerr << "syntax::parse_node::list_size = " << syntax::parse_node::list_size << std::endl;
    
    grammar G("Grammar");
    lexer   Lexer("Lexer");
    
    lexical::scanner &scan = Lexer.first();
    scan.make( "BLANKS", "[ \t]+", & scan.discard );
    scan.make( "LBRACK", "\\[");
    scan.make( "RBRACK", "\\]");
    scan.make( "INT", "[:digit:]+" );
    scan.make( "ID",  "[:word:]+");
    scan.make( "COMA", ",");
    scan.make( "APOS", "'");
    scan.make( "ENDL", "[:endl:]", & scan.newline );
    
    {
        syntax::aggregate & List   = G.agg("List");      // root
        syntax::terminal  & LBRACK = G.term( "LBRACK", false);
        syntax::terminal  & RBRACK = G.term( "RBRACK", false );
        syntax::alternate & ITEM   = G.alt("ITEM");
        syntax::terminal  & INT    = G.term("INT");
        syntax::terminal  & ID     = G.term("ID");
        syntax::terminal  & APOS   = G.term("APOS");
        ITEM( INT  );
        ITEM( ID   );
        ITEM( List );
        
        syntax::aggregate & TAIL = G.agg("TAIL",true);
        TAIL( G.term("COMA",false) );
        TAIL( ITEM           );
        
        List( LBRACK );
        List( ITEM   );
        List( G.rep( "REPTAIL", TAIL,0 ) );
        List( RBRACK );
        List( G.opt( "OPTTRN", APOS ) );
    }
    
    
    
    ios::icstream fp( ios::cstdin );
    regex::source Source( fp );
    auto_ptr<syntax::parse_node> Tree( G.accept(Lexer, Source) );
    if( Tree.__get() )
    {
        std::cerr << std::endl;
        std::cerr << "Saving Full Parse Tree" << std::endl;
        {
            ios::ocstream fp( "g.dot", false );
            Tree->graphviz("G", fp);
        }
        system( "dot -Tpng g.dot -o g.png" );
        std::cerr << "Compressing Tree" << std::endl;
        Tree->compress();
        std::cerr << "Saving Final Parse Tree" << std::endl;
        {
            ios::ocstream fp( "q.dot", false );
            Tree->graphviz("Q", fp);
        }
        system( "dot -Tpng q.dot -o q.png" );
    }
    
}
YOCTO_UNIT_TEST_DONE()
