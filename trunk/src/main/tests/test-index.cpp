#include "yocto/utest/run.hpp"
#include "yocto/sort/index.hpp"
#include "yocto/sequence/vector.hpp"

#include "support.hpp"
#include <typeinfo>

using namespace yocto;

template <typename T>
static inline void test_index()
{
	const size_t N = 1 + alea_leq(10);
	vector<T>      ra( N, as_capacity );
	vector<size_t> idx(N,0);
	
	std::cerr << "generating " << N << " " << typeid(T).name() << std::endl;
	for( size_t i=0; i < N; ++i )
	{
		T tmp( gen<T>::get() );
		ra.push_back(tmp);
	}
	
	std::cerr << "indexing..." << std::endl;
	make_index( ra, idx, __compare<T> );
	
	for( size_t i=1; i <= N; ++i )
	{
		std::cerr << ra[ idx[i] ] << std::endl;
	}
	
}

YOCTO_UNIT_TEST_IMPL(index)
{

	test_index<int>();
	test_index<float>();
	test_index<string>();
	
}
YOCTO_UNIT_TEST_DONE()
