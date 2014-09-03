#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"


using namespace yocto;
using namespace lang;

class MyScanner : public lexical:: scanner
{
public:
    explicit MyScanner(int &lr ) : lexical::scanner( "Main", lr )
    {
        
        p_dict &d = dict();
        d.add("INT", "[:digit:]+");
        make("ID", "[:alpha:][:word:]*", this, & MyScanner::OnId );
        forward("INT", "{INT}");
        forward("DBL", "{INT}[.]{INT}?");
        forward("FLT", "{INT}[.]{INT}?f");
        discard("BLANKS", "[:blank:]+");
        make("ENDL","[:endl:]",this, & lexical::scanner::newline);
        
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
    MyScanner scan(line);
   
    
    ios::icstream fp( ios::cstdin );
    source        src;
    
    lexemes lxm;
    bool    ctl = false;
    scan.echo   = true;
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
