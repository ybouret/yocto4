#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(scanner)
{
    int              line=1;
    lexical::scanner scan("scanner",line);

    scan.emit("word","[:alpha:][:word:]*");
    scan.drop("blank","[:blank:]");
    
}
YOCTO_UNIT_TEST_DONE()
