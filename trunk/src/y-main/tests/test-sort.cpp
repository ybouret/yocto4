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
        netsort2(ra[1], ra[2]);
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
        netsort3(ra[1], ra[2], ra[3]);
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
        netsort4(ra[1], ra[2], ra[3], ra[4]);
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
        netsort6(ra[1], ra[2], ra[3], ra[4], ra[5], ra[6]);
        std::cerr << ra;
        std::cerr << std::endl;
        check_sorted(ra, "netsort6");
    }

}


YOCTO_UNIT_TEST_IMPL(sort)
{
	test_sort<int>();
	test_sort<float>();
	test_sort<string>();
	test_cosort<int,string>();
    test_netsort<float>();
}
YOCTO_UNIT_TEST_DONE()
