#include "yocto/utest/run.hpp"
#include "yocto/threading/crew.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/shared-ptr.hpp"

using namespace yocto;

class Sum
{
public:
    
    const array<double> *pA, *pB;
    array<double>       *pC;
    
    inline Sum() :
    pA(0), pB(0), pC(0)
    {
    }
    
    inline ~Sum() throw()
    {
    }
    
    
    void run( threading::crew::context &ctx )
    {
        //{ scoped_lock guard( ctx.access ); std::cerr << "Sum::run " << ctx.rank << std::endl; }
        
        assert(pA);
        assert(pB);
        assert(pC);
        
        const threading::crew::window &w = ctx.data.as<threading::crew::window>();
        const array<double> &A = *pA;
        const array<double> &B = *pB;
        array<double>       &C = *pC;
        
        const size_t i0 = w.start;
        const size_t i1 = w.final;
        for( size_t iter=1; iter <= 10; ++iter)
        {
            for( size_t i=i1; i >= i0; --i )
            {
                C[i] = A[i] + B[i];
            }
        }
    }
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Sum);
};

#include "yocto/code/rand.hpp"

YOCTO_UNIT_TEST_IMPL(crew)
{
    
    size_t N=10000;
    
    {
        threading::crew nope;
    }
    
    threading::crew mt;
    mt.dispatch<threading::crew::window>(N, 1);
    for(size_t i=0;i<mt.size;++i)
    {
        const threading::crew::window &w = mt[i].as<threading::crew::window>();
        std::cerr << "\t-- thread #" << mt.size << "." << i << " : " << w.start << " -> " << w.final << " #=" << w.count << std::endl;
    }
    
    std::cerr << "Now in main program" << std::endl;
    Sum s;
    
    threading::crew::task t( &s, & Sum::run );
    
    vector<double> A(N,0), B(N,0), C(N,0);
    s.pA = &A;
    s.pB = &B;
    s.pC = &C;
    
    
    for( size_t i=1; i <= N; ++i )
    {
        A[i] = 100 * ( 0.5 - alea<double>() );
        B[i] = 100 * ( 0.5 - alea<double>() );
    }
    
    const size_t CYCLES=1000;
    wtime chrono;
    chrono.start();
    for( size_t cycle=1; cycle <= CYCLES; ++cycle )
    {
        mt.run(t);
    }
    const double ell = chrono.query();
    std::cerr << std::endl << "Speed= " << (CYCLES/ell)/1000.0 << " kcycles/s" << std::endl << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/string/conv.hpp"

YOCTO_UNIT_TEST_IMPL(cwin)
{
    size_t length = 1000;
    if(argc>1)
        length = strconv::to<size_t>(argv[1],"length");
    
    threading::crew team;
    for(size_t i=0;i<team.size;++i)
    {
        threading::crew::context       &ctx = team[i];
        const threading::crew::window   win(ctx,length,0);
        std::cerr << "win #" << i << " : " << win.start << " => " << win.final << ", #=" << win.count << std::endl;
        ctx.data.make<threading::crew::window>(win);
    }
    
    team.dispatch<threading::crew::window>(length,1);
    
    
}
YOCTO_UNIT_TEST_DONE()
