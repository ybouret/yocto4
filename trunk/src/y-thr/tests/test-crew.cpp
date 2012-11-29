#include "yocto/utest/run.hpp"
#include "yocto/threading/crew.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/wtime.hpp"

using namespace yocto;

class Sum
{
public:
    vector<size_t>       ini;
    vector<size_t>       end;
    const array<double> *pA, *pB;
    array<double>   *pC;
    
    inline Sum() : ini(), end(), pA(0), pB(0), pC(0)
    {
        
    }
    
    inline ~Sum() throw()
    {
    }
    
    void prepare( size_t size, size_t N )
    {
        ini.make(size,0);
        end.make(size,0);
        size_t i = 1;
        for( size_t rank = 0; rank < size; ++rank )
        {
            const size_t todo = N / ( size-rank );
            ini[rank+1] = i;
            i += todo;
            end[rank+1] = i-1;
            N -= todo;
            std::cerr << "rank " << rank << ": " << ini[rank+1] << " -> " << end[rank+1] << std::endl;
        }
    }
    
    void run( threading::crew::context &ctx )
    {
        //{ scoped_lock guard( ctx.access ); std::cerr << "Sum::run " << ctx.rank << std::endl; }
        
        assert(pA);
        assert(pB);
        assert(pC);
        const array<double> &A = *pA;
        const array<double> &B = *pB;
        array<double>       &C = *pC;
        
        const size_t i0 = ini[ ctx.indx ];
        const size_t i1 = end[ ctx.indx ];
        for( size_t i=i1; i >= i0; --i )
            C[i] = A[i] + B[i];
    
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
    Sum s;
    threading::crew::task t( &s, & Sum::run );
    
    vector<double> A(N,0), B(N,0), C(N,0);
    s.pA = &A;
    s.pB = &B;
    s.pC = &C;
    s.prepare( mt.size, N );
    
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
