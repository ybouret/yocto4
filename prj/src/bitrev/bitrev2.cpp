#include "yocto/sequence/vector.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline
void bitrev_loop( T *arr, size_t size )
{
    assert( is_a_power_of_two(size) );
    const size_t n    = size << 1;
    size_t j=1;
    for (size_t i=1; i<n; i+=2)
    {
        if (j > i)
        {
            core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
        }
        size_t m = size; // m=  n / 2;
        while (m >= 2 && j > m)
        {
            j -=  m;
            m >>= 1;
        }
        j += m;
    }
    
}

#define USE_CODE
#if defined(USE_CODE)
#define real_t double
#include "bitrevcode.cxx"
#include "bitrevcode2.cxx"
#endif

static inline void test( size_t size )
{
    typedef complex<double> cplx;
    vector< cplx > u(size,as_capacity);
    for(size_t i=1;i<=size;++i)
    {
        const cplx c( i,i );
        u.push_back(c);
    }
    
    vector< cplx > v(u);
    vector< cplx > w(u);
    vector< cplx > z(u);

    const size_t n = size << 1;
    bitrev_loop( (&v[1].re) - 1, size );
#if defined(USE_CODE)
    __bitrev( (&z[1].re) - 1, size,n);
#endif
    for(size_t i=1;i<=size;++i)
    {
       
#if defined(USE_CODE)
        if( v[i] != z[i] )
            throw exception("BitRev Code Failure for size=%u !", unsigned(size) );
#endif
        
    }
#if defined(USE_CODE)
    for(size_t i=1;i<=size;++i)
    {
         w[i] = z[i] = u[i];
    }
    __bitrev( (&w[1].re) - 1, (&z[1].re) - 1, size,n);
    for(size_t i=1;i<=size;++i)
    {
      
        if(v[i]!=w[i])
            throw exception("BitRev2 Failure 1!");

        if(v[i]!=z[i])
            throw exception("BitRev2 Failure 2!");
    }
#endif
    
    
    wtime chrono;
    chrono.start();
    const size_t ITER = 10000;
    for(size_t iter=0;iter<ITER;++iter)
    {
        bitrev_loop( & v[1].re, size );
    }
    const double ell_loop = chrono.query();
    chrono.start();
    for(size_t iter=0;iter<ITER;++iter)
    {
#if defined(USE_CODE)
        __bitrev(& v[1].re, size,n );
#endif
    }
    const double ell_code = chrono.query();
    const double code_up  = ell_loop/ell_code;
    std::cerr << "code_up" << size << " = " << code_up << std::endl;

}

int main(int argc, char *argv[] )
{
    
    try
    {
        for(size_t p=2;p<=12;++p)
        {
            const size_t size = 1 << p;
            test(size);
        }
        
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Error" << std::endl;
    }
    return 1;
}