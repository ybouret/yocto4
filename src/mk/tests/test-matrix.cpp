#include "yocto/utest/run.hpp"
#include "yocto/container/matrix.hpp"
#include "../../main/tests/support.hpp"
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
		const size_t row = 1 + alea_leq(128);
		const size_t col = 1 + alea_leq(128);
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
        H.set();
        H.run( &m2[1][1], m2.rows*m2.cols*sizeof(T));
		const uint64_t k3 = H.key<uint64_t>();
		if( k1 != k3 )
		{
			throw exception("data no match, after assign!");
		}
        
        matrix<T> mt(m2,YOCTO_MATRIX_TRANSPOSE);
        H.set();
        for(size_t j=1; j <= mt.cols; ++j)
        {
            for(size_t i=1;i<=mt.rows;++i)
            {
                H.run( &mt[i][j], sizeof(T));
            }
        }
        const uint64_t k4 = H.key<uint64_t>();
		if( k1 != k4 )
		{
			throw exception("data no match, after transpose!");
		}
        
        mt.transpose();
        H.set();
        H.run( &mt[1][1], mt.rows*mt.cols*sizeof(T));
        const uint64_t k5 = H.key<uint64_t>();
		if( k1 != k5 )
		{
			throw exception("data no match, after 2 transpose!");
		}

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
