#include "yocto/utest/run.hpp"
#include "yocto/sort/heap.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/comparator.hpp"

#include "support.hpp"

using namespace yocto;

template <typename T>
static inline void check_sorted( const array<T> &ra, const char *ctx)
{
    assert(ctx);
    for(unsigned j=2;j<=ra.size();++j)
    {
        if(ra[j]<ra[j-1]) throw exception("%s: error at indices %u/%u", ctx, j-1,j);
    }
}


template <typename T>
static inline void test_sort()
{
	std::cerr << "-- sorting" << std::endl;
	const size_t n = 1 + alea_leq( 20 );
	vector<T>    ra(n,as_capacity);
	for( size_t i=0; i < n; ++i )
	{
		T tmp( gen<T>::get() );
		ra.push_back( tmp );
	}
	std::cerr << "raw=" << ra << std::endl;
	hsort( ra, __compare<T> );
	std::cerr << "srt=" << ra << std::endl;
    check_sorted(ra, "sorting");
}

template <typename T,typename U>
static inline void test_cosort()
{
	std::cerr << "-- co-sorting" << std::endl;
	const size_t n = 1 + alea_leq( 20 );
	vector<T>    ra(n,as_capacity);
	vector<U>    rb(n,as_capacity);
	for( size_t i=0; i < n; ++i )
	{
		{
			T tmp( gen<T>::get() );
			ra.push_back( tmp );
		}
		{
			U tmp( gen<U>::get() );
			rb.push_back( tmp );
		}
	}
	std::cerr << "rawa=" << ra << std::endl;
	std::cerr << "rawb=" << rb << std::endl;
	hsort( ra, rb, __compare<T> );
	std::cerr << "srta=" << ra << std::endl;
	std::cerr << "srtb=" << rb << std::endl;
    check_sorted(ra, "co-sorting");
}

#include "yocto/sort/network.hpp"

template <typename T>
static inline void test_netsort()
{
    std::cerr << "-- sorting network" << std::endl;

    const size_t n = 1 + alea_leq( 20 );
    vector<T> ra(6,as_capacity);
    
    //-- level 2
    for(size_t i=0;i<n;++i)
    {
        ra.free();
        for(size_t j=0;j<2;++j) { const T tmp( gen<T>::get() ); ra.push_back(tmp); }
        std::cerr << ra << " => ";
        netsort<T>::level2(ra[1], ra[2]);
        std::cerr << ra;
        std::cerr << std::endl;
        check_sorted(ra, "netsort2");
    }
    
    //-- level 3
    for(size_t i=0;i<n;++i)
    {
        ra.free();
        for(size_t j=0;j<3;++j) { const T tmp( gen<T>::get() ); ra.push_back(tmp); }
        std::cerr << ra << " => ";
        netsort<T>::level3(ra[1], ra[2], ra[3]);
        std::cerr << ra;
        std::cerr << std::endl;
        check_sorted(ra, "netsort3");
    }
    
    //-- level 4
    for(size_t i=0;i<n;++i)
    {
        ra.free();
        for(size_t j=0;j<4;++j) { const T tmp( gen<T>::get() ); ra.push_back(tmp); }
        std::cerr << ra << " => ";
        netsort<T>::level4(ra[1], ra[2], ra[3], ra[4]);
        std::cerr << ra;
        std::cerr << std::endl;
        check_sorted(ra, "netsort4");
    }
    
    //-- level 6
    for(size_t i=0;i<n;++i)
    {
        ra.free();
        for(size_t j=0;j<6;++j) { const T tmp( gen<T>::get() ); ra.push_back(tmp); }
        std::cerr << ra << " => ";
        netsort<T>::level6(ra[1], ra[2], ra[3], ra[4], ra[5], ra[6]);
        std::cerr << ra;
        std::cerr << std::endl;
        check_sorted(ra, "netsort6");
    }

}


template <typename T, typename U>
static inline void test_co_netsort()
{
    std::cerr << "-- co_sorting network" << std::endl;

    const size_t n = 1 + alea_leq( 20 );
    vector<T> ra(6,as_capacity);
    vector<U> rb(6,as_capacity);
    
    for(size_t i=0;i<n;++i)
    {
        ra.free();
        rb.free();
        for(size_t j=0;j<2;++j) {
            const T tmp( gen<T>::get() ); ra.push_back(tmp);
            const U ump( gen<U>::get() ); rb.push_back(ump);
        }
        std::cerr << "rawA=" << ra << std::endl;
        std::cerr << "rawB=" << rb << std::endl;
        netsort<T>::template co_level2<U>( &ra[1], &rb[1]);
        std::cerr << "srtA=" << ra << std::endl;
        std::cerr << "srtB=" << rb << std::endl;
        check_sorted(ra, "co_netsort2");
    }

    
    for(size_t i=0;i<n;++i)
    {
        ra.free();
        rb.free();
        for(size_t j=0;j<3;++j) {
            const T tmp( gen<T>::get() ); ra.push_back(tmp);
            const U ump( gen<U>::get() ); rb.push_back(ump);
        }
        std::cerr << "rawA=" << ra << std::endl;
        std::cerr << "rawB=" << rb << std::endl;
        netsort<T>::template co_level3<U>( &ra[1], &rb[1]);
        std::cerr << "srtA=" << ra << std::endl;
        std::cerr << "srtB=" << rb << std::endl;
        check_sorted(ra, "co_netsort3");
    }

    
}


YOCTO_UNIT_TEST_IMPL(sort)
{
	test_sort<int>();
	test_sort<float>();
	test_sort<string>();
	test_cosort<int,string>();
    test_netsort<float>();
    test_co_netsort<int, float>();
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sort/quick.hpp"
template <typename T>
static inline void test_qsort()
{
	std::cerr << "-- sorting" << std::endl;
	const size_t n = 1 + alea_leq( 20 );
	vector<T>    ra(n,as_capacity);
	for( size_t i=0; i < n; ++i )
	{
		T tmp( gen<T>::get() );
		ra.push_back( tmp );
	}
	std::cerr << "raw=" << ra << std::endl;
	quicksort( ra, __compare<T> );
	std::cerr << "srt=" << ra << std::endl;
    check_sorted(ra, "sorting");
}

template <typename T,typename U>
static inline void test_coqsort()
{
	std::cerr << "-- co-quicksorting" << std::endl;
	const size_t n = 1 + alea_leq( 20 );
	vector<T>    ra(n,as_capacity);
	vector<U>    rb(n,as_capacity);
	for( size_t i=0; i < n; ++i )
	{
		{
			T tmp( gen<T>::get() );
			ra.push_back( tmp );
		}
		{
			U tmp( gen<U>::get() );
			rb.push_back( tmp );
		}
	}
	std::cerr << "rawa=" << ra << std::endl;
	std::cerr << "rawb=" << rb << std::endl;
	co_qsort( ra, rb, __compare<T> );
	std::cerr << "srta=" << ra << std::endl;
	std::cerr << "srtb=" << rb << std::endl;
    check_sorted(ra, "co-sorting");
}


YOCTO_UNIT_TEST_IMPL(qsort)
{
    test_qsort<int>();
    test_qsort<float>();
    test_qsort<double>();
    test_qsort<string>();
    test_coqsort<string,int>();
    test_coqsort<int,string>();
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sys/wtime.hpp"

#if defined(NDEBUG)
#define ITER_MAX 2048
#else
#define ITER_MAX 1024
#endif

#include <cstdlib>

enum SortWith
{
    HeapSort,
    QuickSort,
    LibcSort
};

template <typename T>
inline int compare_args( const void *lhs, const void *rhs ) throw()
{
    return __compare( *(const T*)lhs, *(const T*)rhs);
}

template <typename T>
inline double test_perf( size_t n, SortWith proc )
{
    wtime chrono;
    chrono.start();
    
    vector<T> arr(n,as_capacity);
    for(size_t i=0;i<n;++i) { const T tmp( gen<T>::get() ); arr.push_back(tmp); }
    double average = 0;
    for(size_t i=0;i<ITER_MAX;++i)
    {
        c_shuffle( &arr[1], n);
        const double stamp = chrono.query();
        switch(proc)
        {
            case HeapSort:
                hsort(arr);
                break;
                
            case QuickSort:
                quicksort(arr);
                break;
                
            case LibcSort:
                qsort(&arr[1],n,sizeof(T),compare_args<T>);
                break;
        }
        average += chrono.query() - stamp;
        check_sorted(arr,"sorting perf");
    }
    return average/ITER_MAX;
}

#include "yocto/ios/ocstream.hpp"

template <typename T>
inline void compare_perf(size_t nmax, const char *filename )
{
    std::cerr << std::endl << filename << std::endl;
    ios::ocstream::overwrite(filename);
    for(size_t n=4;n<=nmax;n += 4)
    {
        (std::cerr << ".").flush();
        const double htmx = test_perf<T>(n,HeapSort);
        const double qtmx = test_perf<T>(n,QuickSort);
        const double ctmx = test_perf<T>(n,LibcSort);
        ios::ocstream fp(filename,true);
        fp("%g %g %g %g\n", double(n), htmx, qtmx, ctmx);
    }
    std::cerr << std::endl;
    
}


YOCTO_UNIT_TEST_IMPL(sort_perf)
{
    const size_t nmax = 256;
    compare_perf<int>(nmax,"perf_int.dat");
    compare_perf<float>(nmax,"perf_flt.dat");
    compare_perf<double>(nmax,"perf_dbl.dat");
    compare_perf<string>(nmax,"perf_str.dat");

}
YOCTO_UNIT_TEST_DONE()


