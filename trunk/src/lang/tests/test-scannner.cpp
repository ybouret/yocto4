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
    }
    
    virtual ~MyScanner() throw()
    {
    }
    
    
private:
    
};

YOCTO_UNIT_TEST_IMPL(scanner)
{
    int       line = 0;
    MyScanner scan(line);
    
}
YOCTO_UNIT_TEST_DONE()
