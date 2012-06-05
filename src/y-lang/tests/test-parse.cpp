#include "yocto/utest/run.hpp"
#include "yocto/lang/grammar.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(parse)
{
    std::cerr << "sizeof(lexeme)             = " << sizeof(lexeme) << std::endl;
    std::cerr << "sizeof(syntax::parse_node) = " << sizeof(syntax::parse_node) << std::endl;
    
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
        syntax::terminal  & LBRACK = G.term( "LBRACK" );
        syntax::terminal  & RBRACK = G.term( "RBRACK" );
        syntax::alternate & ITEM   = G.alt("ITEM");
        syntax::terminal  & INT    = G.term("INT");
        syntax::terminal  & ID     = G.term("ID");
        syntax::terminal  & APOS   = G.term("APOS");
        ITEM( INT  );
        ITEM( ID   );
        ITEM( List );
        
        syntax::aggregate & TAIL = G.agg("TAIL");
        TAIL( G.term("COMA") );
        TAIL( ITEM           );
        
        List( LBRACK );
        List( ITEM   );
        List( G.rep( "REPTAIL", TAIL,0 ) );
        List( RBRACK );
        List( G.opt( "OPTTRN", APOS ) );
    }
    
    
    
    ios::icstream fp( ios::cstdin );
    regex::source Source( fp );
    if( G.accept(Lexer, Source) )
    {
        
    }
}
YOCTO_UNIT_TEST_DONE()
