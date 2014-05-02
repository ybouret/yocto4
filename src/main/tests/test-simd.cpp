#include "yocto/utest/run.hpp"
#include "yocto/threading/simd.hpp"
#include "yocto/threading/window.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"

#include "yocto/string/conv.hpp"

#include <cmath>

using namespace yocto;
using namespace threading;

namespace  {
    
    
    static inline
    void DoSomething( context &ctx )
    {
        YOCTO_LOCK(ctx.access);
        std::cerr << "working from " << ctx.size << "." << ctx.rank << std::endl;
    }
    
    class Sum
    {
    public:
        explicit Sum() : pA(0), pB(0), pC(0) {}
        virtual ~Sum() throw() {}
        
        const array<double> *pA;
        const array<double> *pB;
        array<double>       *pC;
        
        void run( const context &ctx )
        {
            assert(pA);
            assert(pB);
            assert(pC);
            const array<double> &A = *pA;
            const array<double> &B = *pB;
            array<double>       &C = *pC;
            
            const window &win = ctx.as<window>();
            for(size_t i=win.start;i<=win.final;++i)
            {
                const double a = A[i];
                const double b = B[i];
                C[i] = sqrt(a*a+b*b);
            }
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Sum);
    };
    
}

YOCTO_UNIT_TEST_IMPL(simd)
{
    
    size_t N = 1000;
    if( argc > 1 )
    {
        N = strconv::to<size_t>( argv[1], "N" );
    }
    
    size_t CYCLES = 100;
    if( argc > 2 )
    {
        CYCLES = strconv::to<size_t>( argv[2], "CYCLES" );
    }
    
    {
        SIMD simd0;
    }
    
    SIMD simd;
    
    SIMD::Kernel K = cfunctor( DoSomething );
    for(size_t cycle=1+alea_leq(3);cycle>0;--cycle)
    {
        simd(K);
    }
    
    Sum sum;
    SIMD::Kernel kSum( &sum, & Sum::run );
    
    vector<double> A(N,0);
    vector<double> B(N,0);
    vector<double> C(N,0);
    
    for(size_t i=1; i<=N;++i)
    {
        A[i] = 0.5 - alea<double>();
        B[i] = 0.5 - alea<double>();
    }
    
    
    sum.pA = &A;
    sum.pB = &B;
    sum.pC = &C;
    
    simd.dispatch<window>(N,1);
    for(size_t rank=0;rank<simd.size;++rank)
    {
        const window &w = simd[rank].as<window>();
        std::cerr << "rank=" << rank << ": " << w.start << " -> " << w.final << " #" << w.count << std::endl;
    }
    
    
    
    single_context mono;
    mono.create<window>(N,1);
    
    
    wtime chrono;
    chrono.start();
    for(size_t i=0;i<CYCLES;++i)
    {
        sum.run(mono);
    }
    const double tmx0 = chrono.query() / CYCLES;
    std::cerr << "tmx0=" << tmx0 << std::endl;
    
    chrono.start();
    for(size_t i=0; i< CYCLES; ++i )
    {
        simd( kSum );
    }
    const double tmx = chrono.query()/CYCLES;
    std::cerr << "tmx=" << tmx << std::endl;
    
    std::cerr << std::endl;
    std::cerr << "SpeedUp=" << tmx0/tmx << std::endl;
    std::cerr << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
