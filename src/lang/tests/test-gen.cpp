#include "yocto/utest/run.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/graphviz.hpp"

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


    if(argc>1)
    {
        auto_ptr<parser> P;

        {
            ios::icstream fp( argv[1] );
            P.reset( parser::generate(vfs::get_base_name(argv[1]),fp,true) );
        }

        std::cerr << "Ready" << std::endl;
        
        try
        {
            ios::icstream           fp( ios::cstdin );
            auto_ptr<syntax::xnode> tree( P->run(fp) );
            if(tree.is_valid())
            {
                tree->graphviz("gen.dot");
                ios::graphviz_render("gen.dot");
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
