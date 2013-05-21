#include "yocto/utest/run.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/fs/local-fs.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lingua;


namespace
{
    
    class MyJSON : public parser
    {
    public:
        explicit MyJSON() : parser( "JSON", "main" )
        {
        }
        
        virtual ~MyJSON() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyJSON);
        
    };
    
}

YOCTO_UNIT_TEST_IMPL(json)
{
    MyJSON json;
    source src;
    src.attach_stdin();
    
    
    
}
YOCTO_UNIT_TEST_DONE()
