#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(scanner)
{
    int line=1;
    lexical::scanner scan("dummy scan",line);
    //scan.echo = true;

    scan.emit("ID",    "[:alpha:]+");
    scan.emit("INT",   "[:digit:]+");
    scan.emit("LONG",  "[:digit:]+[lL]");
    scan.make("WS",    "[:blank:]+", &scan, &lexical::scanner::discard);
    scan.make("ENDL",  "[:endl:]",   &scan, &lexical::scanner::newline);
    scan.make("other", ".",          &scan, &lexical::scanner::forward);


    l_list lexemes;
    lexeme *lx = 0;

    ios::icstream fp(ios::cstdin);
    lang::source  src;

    while( NULL !=(lx=scan.get(src,fp) ) )
    {
        lexemes.push_back(lx);
    }
}
YOCTO_UNIT_TEST_DONE()

