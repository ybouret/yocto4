#include "yocto/utest/run.hpp"
#include "yocto/threading/simd.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"

#include "yocto/string/conv.hpp"

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
            
            const SIMD::Window win(ctx,A.size(),1);
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

#include "yocto/threading/split.hpp"
namespace
{
    template <typename T>
    static inline
    void __test_split1D()
    {
        const T global_offset(1);
        const T global_length(100);

        for(size_t size=1;size<=6;++size)
        {
            std::cerr << std::endl << "size=" << size << ": " << global_offset << " -> " << global_length+global_offset-1  << std::endl;
            for(size_t rank=0;rank<size;++rank)
            {
                std::cerr << "\t" << size << "." << rank << ": ";
                T offset = global_offset;
                T length = global_length;
                parallel_split(rank, size, offset, length);
                std::cerr << offset << " -> " << length + offset -1 << " / length=" <<  length;
                std::cerr << std::endl;
            }
        }
    }

}
YOCTO_UNIT_TEST_IMPL(split)
{
    __test_split1D<int>();
    __test_split1D<uint16_t>();
}
YOCTO_UNIT_TEST_DONE()


