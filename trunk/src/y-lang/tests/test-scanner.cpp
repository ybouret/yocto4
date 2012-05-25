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

    scan( "INT",    "{INT}" );
    scan( "DBL",    "{DBL}" );
    scan( "FLT",    "{FLT}" );
    scan( "STR",    "[:cstring:]");
    scan( "BLANKS", "[ \t]",    & scan.discard );
    scan( "ENDL",   "[:endl:]", & scan.newline );
    scan.no_dict();
    
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    lang::lexemes lxs;
    
    lang::lexeme *lx = NULL;
    while( NULL != (lx=scan.next_lexeme(src)) )
    {
		std::cerr << "line: " << scan.line << ": " << lx->label << " [" << *lx << "]" << std::endl;
        lxs.push_back(lx);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
