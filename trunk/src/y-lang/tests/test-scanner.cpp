#include "yocto/utest/run.hpp"
#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/rx/pattern/posix.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(scanner)
{

    lang::lexical::scanner scan("dummy");
    
    scan.dict().record( "DIGIT", regex::posix::digit() );
    
    scan( "INT",    "{DIGIT}+" );
    scan( "BLANKS", "[ \t]",    & scan.discard );
    scan( "ENDL",   "[:endl:]", & scan.newline );
    scan.no_dict();
    
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    lang::lexemes lxs;
    
    lang::lexeme *lx = NULL;
    while( NULL != (lx=scan.next_lexeme(src)) )
    {
        lxs.push_back(lx);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
