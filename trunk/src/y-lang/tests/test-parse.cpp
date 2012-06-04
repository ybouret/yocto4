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
    
    
    
    syntax::aggregate & __vec = G.agg( "vec" );
    __vec.add( G.term( "[" ) );
    __vec.add( G.term( "]" ) );
}
YOCTO_UNIT_TEST_DONE()
