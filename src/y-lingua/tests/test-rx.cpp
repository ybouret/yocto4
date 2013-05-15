#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/compiler.hpp"
#include "yocto/lingua/pattern/posix.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/imstream.hpp"

#include "yocto/auto-ptr.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lingua;

//void shared_test_motifs( p_list &motifs );

YOCTO_UNIT_TEST_IMPL(rx)
{
    if(argc>1)
    {
        p_dict dict;
        dict.insert("DIGIT", posix::digit() );
        const string      rxp = argv[1];
        auto_ptr<pattern> q( compile(rxp,&dict) );
        
        {
            ios::ocstream fp("expr.bin",false);
            q->save(fp);
        }
        
        q->graphviz( "expr.dot" );
        system("dot -Tpng -oexpr.png expr.dot");
        
        string line;
        ios::icstream fp( ios::cstdin );
        source        src;
        while( line.clear(), (std::cerr << "> ").flush(), fp.read_line(line) > 0 )
        {
            std::cerr << "matching '" << line << "'" << std::endl;
            
            q->reset();
            {
                const input inp( new ios::imstream(line) );
                src.attach(inp);
            }
            if(q->accept(src))
            {
                std::cerr << "accept '" << *q << "'" << std::endl;
            }
            else
                std::cerr << "reject" << std::endl;
        }
        
    }
}
YOCTO_UNIT_TEST_DONE()
