#include "yocto/utest/run.hpp"
#include "yocto/counting/combination.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(comb)
{
    
    
    size_t n = (argc > 1) ? strconv::to<size_t>(argv[1],"n") : 10 ;
    size_t k = (argc > 2) ? strconv::to<size_t>(argv[2],"k") :  3 ;
    combination C(n,k);
    
    size_t count = 0;
    do
    {
        ++count;
        std::cerr << C << std::endl;
    }
    while( C.next() );
    
    std::cerr << "C(" << n << "," << k << ")=" << count << " / " << C.id << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sys/wtime.hpp"
#include "yocto/ios/ocstream.hpp"

YOCTO_UNIT_TEST_IMPL(comb_perf)
{
    wtime chrono;
    ios::ocstream fp("comb_perf.dat",false);
    for(unsigned n=1;n<=5;++n)
    {
        for(unsigned p=1;p<=n;++p)
        {
            combination C(n,p);
            chrono.start();
            double ell = 0;
            size_t num = 0;
            do
            {
                C.count_all();
                ell = chrono.query();
                ++num;
            } while(ell<1);
            const double speed = 1e-6 * double(num)/ell;
            std::cerr << "speed C(" << n << "," << p << ")=" << speed << " M/s" << std::endl;
            
            fp("%u %g\n", p, speed);
        }
        fp("\n");
    }
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/counting/dispatch.hpp"
YOCTO_UNIT_TEST_IMPL(dispatch)
{
    size_t socks   = (argc > 1) ? strconv::to<size_t>(argv[1],"socks") : 1 ;
    size_t drawers = (argc > 2) ? strconv::to<size_t>(argv[2],"drawers") :  3 ;
    dispatch D(socks,drawers);
    do
    {
        std::cerr << "D" <<  D.id() << " = " << D << std::endl;
        }
        while(D.next());
        }
        YOCTO_UNIT_TEST_DONE()
