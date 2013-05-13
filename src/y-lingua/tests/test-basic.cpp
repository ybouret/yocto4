#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(basic)
{
    
    p_list motifs;
    
    motifs.push_back( any1::create()        );
    motifs.push_back( single::create('A')   ) ;
    motifs.push_back( range::create('0','9'));
    {
        within *p = within::create();
        motifs.push_back(p);
        p->append('a');
        p->append('b');
    }
    {
        none *p = none::create();
        motifs.push_back(p);
        p->append('c');
        p->append('d');
    }
    
    p_list motifs_cpy(motifs);
    motifs_cpy.kill();
    
    source src;
    std::cerr << "Enter words:" << std::endl;
    ios::icstream fp( ios::cstdin );
    string line;
    while( fp.read_line(line) >= 0)
    {
        std::cerr << "<" << line << ">" << std::endl;
        {
            const input inp( new ios::imstream( line ) );
            src.attach(inp);
        }
        while( src.is_active() )
        {
            for( pattern *p = motifs.head;p;p=p->next)
            {
                if(p->accept(src) )
                {
                    std::cerr << "accept <" << *p << ">" << std::endl;
                    src.unget( *p );
                    p->reset();
                }
                else
                {
                    std::cerr << "reject" << std::endl;
                }
            }
            assert(src.cache_size()>0);
            src.skip(1);
        }
        line.clear();
    }
}
YOCTO_UNIT_TEST_DONE()
