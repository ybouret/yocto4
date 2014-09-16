#include "yocto/lang/generator.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/fs/local-fs.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lang;


namespace
{
    
    
    
}

YOCTO_UNIT_TEST_IMPL(gen)
{
    
    vfs &fs = local_fs::instance();
    
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");
    
    
    generator     G;
    ios::icstream fp( ios::cstdin );
    if(argc>1 && 0 == strcmp("lex",argv[1]) )
    {
        while(true)
        {
            source src;
            lexeme *lx = G.get( src, fp );
            if(!lx)
                break;
            
            auto_ptr<lexeme> LX(lx);
            std::cerr << lx->label << " : '" << *lx << "'" << std::endl;
        }
    }
    else
    {
        G.process(fp);
    }
    
}
YOCTO_UNIT_TEST_DONE()

