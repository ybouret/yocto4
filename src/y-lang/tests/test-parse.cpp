#include "yocto/utest/run.hpp"
#include "yocto/lang/grammar.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(parse)
{
    std::cerr << "sizeof(lexeme)             = " << sizeof(lexeme) << std::endl;
    std::cerr << "sizeof(syntax::parse_node) = " << sizeof(syntax::parse_node) << std::endl;
    
    grammar G("G");
    lexer   Lexer("G");
    
    lexical::scanner &scan = Lexer.first();
    scan.make( "BLANKS", "[ \t]+", & scan.discard );
    scan.make( "LBRACK", "\\[");
    scan.make( "RBRACK", "\\]");
    scan.make( "INT", "[:digit:]+" );
    scan.make( "ID",  "[:word:]+");
    scan.make( "ENDL", "[:endl:]", & scan.newline );
    
    
    syntax::aggregate & __vec = G.agg( "vec" );
    syntax::alternate & __mid = G.alt( "mid" );
    __mid.add( G.term("INT") );
    __mid.add( G.term("ID")  );
    
    __vec.add( G.term( "LBRACK" ) );
    __vec.add( __mid );
    __vec.add( G.term( "RBRACK" ) );
    
    
    
    ios::icstream fp( ios::cstdin );
    regex::source Source( fp );
    if( G.accept(Lexer, Source) )
    {
        
    }
}
YOCTO_UNIT_TEST_DONE()
