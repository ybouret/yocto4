#include "yocto/utest/run.hpp"
#include "yocto/lang/grammar.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/ios/ocstream.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lang;



YOCTO_UNIT_TEST_IMPL(json)
{
    
    lexer   L("JSON Lexer");
    grammar G("JSON Gramar");
    
    lexical::scanner &scan = L.first();
    
    //-- lexical elements
    scan.make("LBRACK","\\[" );
    scan.make("RBRACK","\\]" );
    scan.make("COMMA",  ","  );
    scan.make("Null",   "null" );
    scan.make("True",   "true" );
    scan.make("False",  "false" );
    scan.make("STRING", "[:cstring:]");
    scan.make("NUMBER", "[:digit:]+" );
    
    scan.make( "BLANKS", "[ \t]+", & scan.discard );
    scan.make( "ENDL", "[:endl:]", & scan.newline );
    
    //-- root = ELEMENT: ARRAY | OBJECT
    syntax::alternate &ELEMENT  = G.alt("ELEMENT");
    
    //-- terminals
    syntax::terminal  &LBRACK    = G.term("LBRACK",false);
    syntax::terminal  &RBRACK    = G.term("RBRACK",false);
    syntax::terminal  &Null      = G.term("Null");
    syntax::terminal  &True      = G.term("True");
    syntax::terminal  &False     = G.term("False");
    syntax::terminal  &COMMA     = G.term("COMMA",false);
    syntax::terminal  &STRING    = G.term("STRING");
    syntax::terminal  &NUMBER    = G.term("NUMBER");
    
    //-- non terminal
    syntax::alternate &ARRAY    = G.alt("ARRAY");
    
    syntax::alternate &VALUE       = G.alt("VALUE");
    VALUE << Null << True << False << STRING << NUMBER;
    
    syntax::aggregate &OTHER_VALUE = G.agg("OTHER_VALUE", true);
    OTHER_VALUE << COMMA << VALUE;
    
    syntax::repeating &OTHER_VALUES = G.rep("OTHER_VALUES", OTHER_VALUE, 0);
    
    syntax::aggregate &EMPTY_ARRAY = G.agg("EMPTY_ARRAY");
    EMPTY_ARRAY << LBRACK << RBRACK;
    
    syntax::aggregate &FILLED_ARRAY = G.agg("FILLED_ARRAY");
    FILLED_ARRAY << LBRACK << VALUE << OTHER_VALUES << RBRACK;
    
    ARRAY << EMPTY_ARRAY << FILLED_ARRAY;
    
    VALUE << ARRAY;
    //-- finally
    ELEMENT << ARRAY;

    ios::icstream fp( ios::cstdin );
    regex::source Source( fp );
    auto_ptr<syntax::parse_node> Tree( G.accept(L, Source) );
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
