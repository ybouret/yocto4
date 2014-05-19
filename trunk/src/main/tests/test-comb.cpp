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

#define __SHOW() std::cerr << "speed C(" << n << "," << p << ")=" << speed << " M/s" << std::endl
YOCTO_UNIT_TEST_IMPL(comb_perf)
{
    
    const char filename[] =
#if defined(NDEBUG)
    "comb_perf_opt.dat"
#else
    "comb_perf_dbg.dat"
#endif
    ;
    
    wtime chrono;
    {
        ios::ocstream fp(filename,false);
    }
    
    const unsigned nmax = 20;
    const unsigned n2   = nmax*nmax;
    
    double total_time = n2; //!< assuming 1s for each C(n,p)
    if(total_time>240)
    {
        total_time = 240;
    }
    const double max_ell = total_time / n2;
    
    
    for(unsigned n=1;n<=20;++n)
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
            } while(ell<max_ell);
            const double speed = 1e-6 * double(num)/ell;
            __SHOW();
            const double tpc   = (ell/num)*1e6;
            {
                ios::ocstream fp(filename,true);
                fp("%u %g\n", p, tpc);
            }
        }
        
        {
            ios::ocstream fp(filename,true);
            fp("\n");
        }
    }
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/counting/dispatch.hpp"
YOCTO_UNIT_TEST_IMPL(dispatch)
{
    size_t socks   = (argc > 1) ? strconv::to<size_t>(argv[1],"socks")   :  1 ;
    size_t drawers = (argc > 2) ? strconv::to<size_t>(argv[2],"drawers") :  3 ;
    dispatch D(socks,drawers);
    do
    {
        (std::cerr << "D" <<  D.id() << " = " << D << std::endl);
    }
    while(D.next());
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(comb_io)
{
    size_t n = (argc > 1) ? strconv::to<size_t>(argv[1],"n") : 10 ;
    size_t k = (argc > 2) ? strconv::to<size_t>(argv[2],"k") :  3 ;
    combination C(n,k);
    std::cerr << "bytes_per_frame(" << n << "," << k <<") = " << C.bytes_per_frame << std::endl;
    const uint64_t card = C.count_all();
    std::cerr << "#combi=" << card << std::endl;
    std::cerr << "bytes =" << card*C.bytes_per_frame << std::endl;
}
YOCTO_UNIT_TEST_DONE()

