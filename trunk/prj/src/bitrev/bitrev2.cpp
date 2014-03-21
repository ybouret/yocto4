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
    --arr;
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

namespace {
#define word_t unsigned
#include "bitrevtab.cxx"
}

#include "yocto/code/unroll.hpp"

#define BITREV(N) for(size_t i=0;i<num##N;++i)  core::bswap<2*sizeof(T)>( &arr[idx##N[i]], &arr[jdx##N[i]] )

#define CASE_RAW(N) case N: BITREV(N); break

#define BITREV_CODE(ii) core::bswap<2*sizeof(T)>( &arr[I[ii]], &arr[J[ii]] )


#define CASE_UNR(N) case N:  YOCTO_LOOP_FUNC_(num##N,BITREV_CODE,0); break

//#define CASE CASE_UNR
#define CASE CASE_RAW

template <typename T>
static inline
void bitrev_case( T *arr, size_t size )
{
    switch (size)
    {
        case 0:
        case 1:
        case 2:
            break;
#define I idx4
#define J jdx4
            CASE(4);
#undef  I
#undef  J
#define I idx8
#define J jdx8
            CASE(8);
#undef I
#undef J
#define I idx16
#define J jdx16
            CASE(16);
#undef I
#undef J
#define I idx32
#define J jdx32
            CASE(32);
#undef I
#undef J
#define I idx64
#define J jdx64
            CASE(64);
#undef I
#undef J
#define I idx128
#define J jdx128
            CASE(128);
#undef I
#undef J
#define I idx256
#define J jdx256
            CASE(256);
#undef I
#undef J
#define I idx512
#define J jdx512
            CASE(512);
#undef I
#undef J
#define I idx1024
#define J jdx1024
            CASE(1024);
#if 0
#undef I
#undef J
#define I idx2048
#define J jdx2048
            CASE(2048);
#undef I
#undef J
#define I idx4096
#define J jdx4096
            CASE(4096);
#endif
        default:
        {
            assert( is_a_power_of_two(size) );
            --arr;
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
            break;
    }
}

#define real_t double
#include "bitrevcode.cxx"

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

    
    bitrev_loop( & v[1].re, size );
    bitrev_case( & w[1].re, size );
    __bitrev( &z[1].re, size);
    
    for(size_t i=1;i<=size;++i)
    {
        if(v[i]!=w[i])
            throw exception("BitRev Failure!");
    }
    
    wtime chrono;
    chrono.start();
    const size_t ITER = 10000;
    for(size_t iter=0;iter<ITER;++iter)
    {
        bitrev_loop( & v[1].re, size );
    }
    const double ell_loop = chrono.query();
    //std::cerr << "\tloop=" << ell_loop << std::endl;
    
    chrono.start();
    for(size_t iter=0;iter<ITER;++iter)
    {
        bitrev_case( & v[1].re, size );
    }
    const double ell_case = chrono.query();
    //std::cerr << "\tcase=" << ell_case << std::endl;
    
    const double case_up = ell_loop/ell_case;
    
    chrono.start();
    for(size_t iter=0;iter<ITER;++iter)
    {
        __bitrev(& v[1].re, size );
    }
    const double ell_code = chrono.query();
    const double code_up  = ell_loop/ell_code;
    std::cerr << "case_up" << size << " = " << case_up << " / " << "code_up" << size << " = " << code_up << std::endl;

}

int main(int argc, char *argv[] )
{
    
    try
    {
        for(size_t p=2;p<=10;++p)
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