#include "yocto/utest/run.hpp"
#include "yocto/sequence/c-array.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(c_array)
{
	int tab[] = { 1, 2, 4, 5 };
	c_array<int> arr( tab, sizeof(tab)/sizeof(tab[0]));
	std::cerr << "arr=" << arr << std::endl;
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/sequence/lw-arrays.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/rand.hpp"

static inline void process_arr( array<float> &k )
{
	std::cerr << "array.size=" << k.size() << std::endl;
	for( size_t i=1; i <= k.size(); ++i )
	{
		k[i] = alea<float>();
	}
}

YOCTO_UNIT_TEST_IMPL(lw_arrays)
{
	lw_arrays<float,memory::global> arrays( 4 );
	
	std::cerr << "#arrays="     << arrays.num_arrays() << std::endl;
	std::cerr << "common_size=" << arrays.size         << std::endl;
	
	array<float> &k1 = arrays.next_array();
	array<float> &k2 = arrays.next_array();
	array<float> &k3 = arrays.next_array();
	array<float> &k4 = arrays.next_array();

	process_arr(k1);
	process_arr(k2);
	process_arr(k3);
	process_arr(k4);
	
	arrays.prepare( 10 );
	
	process_arr(k1);
	process_arr(k2);
	process_arr(k3);
	process_arr(k4);
	

	
}
YOCTO_UNIT_TEST_DONE()
