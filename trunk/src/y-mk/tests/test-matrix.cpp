#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "../../y-main/tests/support.hpp"
#include "yocto/hashing/sha1.hpp"

#include <typeinfo>

using namespace yocto;
using namespace math;


template <typename T>
static inline void test_matrix()
{
	hashing::sha1 H;
	{ matrix<T> m; }
	for( size_t iter = 0; iter < 128; ++iter )
	{
		const size_t row = 1 + alea_less_than(128);
		const size_t col = 1 + alea_less_than(128);
		matrix<T> m(row,col);
		//std::cerr << "matrix<" << typeid(matrix<T>).name() << "> " << row << "x" << col << std::endl;
		H.set();
		for( size_t i=1; i <= m.rows; ++i )
		{
			for( size_t j=1; j <= m.cols; ++j )
			{
				const T tmp( gen<T>::get() );
				m[i][j] = tmp;
				H.run( &tmp, sizeof(tmp) );
			}
		}
		const uint64_t k1 = H.key<uint64_t>();
		H.set();
		H.run( &m[1][1], m.rows*m.cols*sizeof(T));
		const uint64_t k2 = H.key<uint64_t>();
		if( k1 != k2 )
		{
			throw exception("data no match!");
		}
		matrix<T> m2 = m;
		m2 = m;
	}
	
}

YOCTO_UNIT_TEST_IMPL(matrix)
{
	test_matrix<float>();
	test_matrix<double>();
	test_matrix< complex<float>  >();
	test_matrix< complex<double> >();
}
YOCTO_UNIT_TEST_DONE()
