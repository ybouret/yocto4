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
    scan.echo = true;

    scan.make("WS",    "[:blank:]+", &scan, &lexical::scanner::discard);
    scan.make("ENDL",  "[:endl:]",   &scan, &lexical::scanner::newline);
    scan.make("other", ".",          &scan, &lexical::scanner::forward);

}
YOCTO_UNIT_TEST_DONE()

