#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"


using namespace yocto;
using namespace lang;

class MyScanner : public lexical:: scanner
{
public:
    explicit MyScanner(int &lr, p_dict &d ) : lexical::scanner( "Main", lr, d)
    {
        
        d.add("INT", "[:digit:]+");
        
        make("ID",     "[:alpha:][:word:]*", this, & MyScanner::OnId );
        make("INT",    "{INT}",              this, &scanner::forward);
        make("DBL",    "{INT}[.]{INT}?",     this, &scanner::forward);
        make("FLT",    "{INT}[.]{INT}?f",    this, &scanner::forward);
        make("BLANKS", "[:blank:]+",         this, &scanner::discard);
        make("ENDL",   "[:endl:]",           this, &scanner::discard_newline);
    }
    
    virtual ~MyScanner() throw()
    {
    }
    
    inline bool OnId( const token &tkn ) throw()
    {
        
        return true;
    }
    
    
private:
    
};

YOCTO_UNIT_TEST_IMPL(scanner)
{
    int       line = 1;
    p_dict    dict;
    MyScanner scan(line,dict);
   
    
    ios::icstream fp( ios::cstdin );
    source        src;
    
    lexemes lxm;
    bool    ctl = false;
    scan.reset();
    
    for(;;)
    {
        lexeme *lx = scan.get(src, fp, ctl );
        if(!lx)
            break;
        std::cerr << lx->label << ":["<< *lx << "]"<< std::endl;
        lxm.push_back(lx);
        lxm.kill();
    }
}
YOCTO_UNIT_TEST_DONE()