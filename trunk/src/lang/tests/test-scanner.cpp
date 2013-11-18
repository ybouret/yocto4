#include "yocto/utest/run.hpp"
#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(scanner)
{

    size_t line = 0;
    lang::lexical::scanner scan("dummy-scanner",line);
    
    scan.dict().record( "DIGIT", "[0-9]" );
    scan.dict().record( "INT",   "{DIGIT}+");
    scan.dict().record( "DBL",   "{INT}[.]{DIGIT}*");
    scan.dict().record( "FLT",   "{DBL}f");

    scan.make( "INT",    "{INT}" );
    scan.make( "DBL",    "{DBL}" );
    scan.make( "FLT",    "{FLT}" );
    scan.make( "STR",    "[:cstring:]");
    scan.make( "BLANKS", "[ \t]",    & scan.discard );
    scan.make( "ENDL",   "[:endl:]", & scan.no_endl );
    scan.no_dict();
    
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    lang::lexemes lxs;
    
    lang::lexeme *lx = NULL;
    
    scan.reset();
    bool fctl = false;
    while( NULL != (lx=scan.next_lexeme(src,fctl)) )
    {
		std::cerr << "line: " << scan.line << ": " << lx->label << " [" << *lx << "]" << std::endl;
        lxs.push_back(lx);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
