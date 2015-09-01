#include "yocto/utest/run.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(gen)
{
    vfs &fs = local_fs::instance();
    const string rmDir = ".";
    const string rmDot = "dot";
    const string rmPng = "png";
    fs.remove_files_with_extension_in(rmDir, rmDot);
    fs.remove_files_with_extension_in(rmDir, rmPng);

#if 0
    fs.try_remove_file("xnode.dot");
    fs.try_remove_file("xnode.png");
    fs.try_remove_file("langen.dot");
    fs.try_remove_file("langen.png");
    fs.try_remove_file("lanraw.dot");
    fs.try_remove_file("lanraw.png");
    fs.try_remove_file("ggram.dot");
    fs.try_remove_file("ggram.png");
    fs.try_remove_file("gen.dot");
    fs.try_remove_file("gen.png");
#endif


    if(argc>1)
    {
        auto_ptr<parser> P;

        {
            ios::icstream fp( argv[1] );
            P.reset( parser::generate(vfs::get_base_name(argv[1]),fp,true) );
            //P->gramviz("gram.dot");(void)system("dot -Tpng -o gram.png gram.dot");

        }

        std::cerr << "Ready" << std::endl;
        
        try
        {
            ios::icstream           fp( ios::cstdin );
            auto_ptr<syntax::xnode> tree( P->run(fp) );
            if(tree.is_valid())
            {
                tree->graphviz("gen.dot");
                (void) system("dot -Tpng -o gen.png gen.dot");
            }
        }
        catch(...)
        {
            if(P.is_valid())
            {
                P->dump();
            }
            throw;
        }
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
