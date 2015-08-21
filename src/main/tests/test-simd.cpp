#include "yocto/utest/run.hpp"
#include "yocto/threading/simd.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"

#include "yocto/string/conv.hpp"
#include "yocto/parallel/split.hpp"
#include <cmath>

using namespace yocto;
using namespace threading;

namespace  {
    
    
    static inline
    void DoSomething( SIMD::Context &ctx )
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
        
        void run( const SIMD::Context &ctx )
        {
            assert(pA);
            assert(pB);
            assert(pC);
            const array<double> &A = *pA;
            const array<double> &B = *pB;
            array<double>       &C = *pC;
            
            size_t      offset = 1;
            size_t      length = A.size();
            parallel::split::in1D(ctx.rank, ctx.size, offset, length);
            const size_t final = offset+length;
            for(size_t i=offset;i<final;++i)
            {
                const double a = A[i];
                const double b = B[i];
                C[i] = sqrt(a*a+b*b);
            }
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Sum);
    };
    

    class DummyTask
    {
    public:
        explicit DummyTask(const SIMD::Context &ctx)
        {
            std::cerr << "\t (*) DummyTask for " << ctx.size << "." << ctx.rank << std::endl;
        }

        explicit DummyTask(const SIMD::Context &ctx, int a)
        {
            std::cerr << "\t (*) DummyTask for " << ctx.size << "." << ctx.rank << " (a=" << a << ")" << std::endl;
        }


        virtual ~DummyTask() throw()
        {
        }

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

    simd.create<int>();
    simd.create<int,int>(2);
    simd.create_from_context<DummyTask>();
    simd.create_from_context<DummyTask,int>(2);

    simd.free();
    simd.release();

    SIMD::Kernel K = cfunctor( DoSomething );
    for(size_t cycle=1+alea_leq(3);cycle>0;--cycle)
    {
        simd(K);
    }

    //return 0;

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
    

    
    SIMD::SingleContext mono;

    
    wtime chrono;
    chrono.start();
    for(size_t i=0;i<CYCLES;++i)
    {
        sum.run(mono);
    }
    const double tmx0 = chrono.query() / CYCLES;
    std::cerr << "tmxST=" << tmx0 << std::endl;
    
    chrono.start();
    for(size_t i=0; i< CYCLES; ++i )
    {
        simd( kSum );
    }
    const double tmx = chrono.query()/CYCLES;
    std::cerr << "tmxMT=" << tmx << std::endl;
    
    std::cerr << std::endl;
    std::cerr << "SpeedUp=" << tmx0/tmx << std::endl;
    std::cerr << std::endl;

}
YOCTO_UNIT_TEST_DONE()



