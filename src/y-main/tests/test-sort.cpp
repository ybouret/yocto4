#include "yocto/utest/run.hpp"
#include "yocto/sort/heap.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/comparator.hpp"

#include "support.hpp"

using namespace yocto;


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
}



YOCTO_UNIT_TEST_IMPL(sort)
{
	test_sort<int>();
	test_sort<float>();
	test_sort<string>();
	test_cosort<int,string>();
}
YOCTO_UNIT_TEST_DONE()
