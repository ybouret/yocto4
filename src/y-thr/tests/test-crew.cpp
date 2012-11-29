#include "yocto/utest/run.hpp"
#include "yocto/threading/crew.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/wtime.hpp"
#include "yocto/shared-ptr.hpp"

using namespace yocto;

class Sum
{
public:
    
    class Worker
    {
    public:
        explicit Worker() : start(0), count(0), final(0)
        {
        }
        ~Worker() throw() {}
        
        typedef shared_ptr<Worker> Ptr;
        
        size_t start, count, final;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Worker);
    };
    
    vector<Worker::Ptr> workers;
    const array<double> *pA, *pB;
    array<double>   *pC;
    
    inline Sum( const threading::crew &tc) :  workers(tc.size,as_capacity), pA(0), pB(0), pC(0)
    {
        for(size_t i=1; i <= tc.size;++i)
        {
            const Worker::Ptr p( new Worker() );
            workers.push_back(p);
        }
    }
    
    inline ~Sum() throw()
    {
    }
    
    void dispatch( const threading::crew &mt, const size_t N )
    {
        mt.dispatch<vector<Worker::Ptr>,size_t>(workers, 1, N);
    }
    
    void display() const
    {
        for( size_t i=1; i <= workers.size();++i)
        {
            const Worker        &w = *workers[i];
            std::cerr << "worker #" << i << ": " << w.start << " -> " << w.final << ": #=" << w.count << std::endl;
        }
    }
    
    void run( threading::crew::context &ctx )
    {
        //{ scoped_lock guard( ctx.access ); std::cerr << "Sum::run " << ctx.rank << std::endl; }
        
        assert(pA);
        assert(pB);
        assert(pC);
        const Worker        &w = *workers[ ctx.indx ];
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
    std::cerr << "Now in main program" << std::endl;
    Sum s(mt);
    s.dispatch(mt, N);
    s.display();
    
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
