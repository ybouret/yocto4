#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/sys/wtime.hpp"
#include <cstdio>

using namespace yocto;
using namespace math;

template <typename T>
static inline double test_copy( size_t N )
{
	const size_t num_iter = 100;
	wtime  chrono;
	double t = 0;
	vector<T> X(N,numeric<T>::zero);
	vector<T> Y(N,numeric<T>::zero);
	for( size_t iter=0; iter < num_iter; ++iter )
	{
		for( size_t i=N;i>1; --i ) 
		{
			const T tmp( alea<float>() );
			X[i] = tmp;
		}
		chrono.start();
		algebra<T>::set(Y,X);
		t += chrono.query();
	}
	return t / num_iter;
}


#define _TEST(NAME,TYPE) fprintf(stderr,"%-8s | %-20s | N=%6d | t=%.4g ms\n", #NAME, #TYPE, int(N), 1000.0 * test_##NAME< TYPE >(N) )

#define _TEST_ALL(NAME) _TEST(NAME,float); _TEST(NAME,double); _TEST(NAME,complex<float>)

YOCTO_UNIT_TEST_IMPL(algebra)
{
	
	for( size_t p=0; p < 18; ++p )
	{
		const size_t N = 1 << p;
		_TEST_ALL(copy);
	}
	
		
}
YOCTO_UNIT_TEST_DONE()


