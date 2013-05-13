#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace lingua;


void shared_test_motifs( p_list &motifs )
{
    vector<string> id;
    for( const pattern *p = motifs.head;p;p=p->next)
    {
        const string tmp = p->make_name();
        id.push_back(tmp);
    }
    
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
            size_t idx = 0;
            size_t lmax = 0;
            for( pattern *p = motifs.head;p;p=p->next)
            {
                ++idx;
                const string &name = id[idx];
                if(p->accept(src) )
                {
                    std::cerr << "accept <" << *p << ">  / [" << name << "]" << std::endl;
                    if(p->size>lmax) lmax = p->size;
                    src.unget( *p );
                    p->reset();
                }
                else
                {
                    std::cerr << "reject / [" << name << "]" << std::endl;
                }
            }
            assert(src.cache_size()>0);
            if(lmax<1) lmax=1;
            std::cerr << "\tskip " << lmax << std::endl;
            src.skip(lmax);
        }
        line.clear();
    }
    
}

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
    
    shared_test_motifs(motifs);
}
YOCTO_UNIT_TEST_DONE()
