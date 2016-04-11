#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(scanner)
{
    int              line=1;
    lexical::scanner scan("scanner",line);

    scan.emit("word","[:alpha:][:word:]*");
    scan.emit("int","[:digit:]+");
    scan.emit("double","[digit]+\\.[:digit:]*");
    scan.drop("punct","[:punct:]");
    scan.drop("blank","[:blank:]");
    scan.endl("endl");

    ios::icstream fp( ios::cstdin );
    source        src(fp);

    l_list  lexemes;
    lexeme *lx=0;
    bool    ctrl=false;
    while( 0 != (lx=scan.get(src,ctrl)))
    {
        std::cerr << lx->line << ":" <<  lx->label << " '" << *lx << "'" << std::endl;
        lexemes.push_back(lx);
    }

}
YOCTO_UNIT_TEST_DONE()
