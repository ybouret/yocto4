#include "yocto/utest/run.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include <typeinfo>
#include "../../main/tests/support.hpp"

#define SHOW(VAR) std::cerr << "numeric<" << name << ">::" #VAR " = " << numeric<T>::VAR << std::endl

using namespace yocto;
using namespace math;

template <typename T>
static inline void show_values( const char *name )
{
    std::cerr << "-- " << name << " -- " << std::endl;
	SHOW(epsilon);
	SHOW(sqrt_epsilon);
	SHOW(minimum);
	SHOW(maximum);
	SHOW(pi);
	SHOW(two_pi);
	SHOW(dig);
	SHOW(ftol);
    SHOW(sqrt_ftol);
	SHOW(min_exp);
	SHOW(max_exp);
    SHOW(tiny);
    SHOW(sqrt_tiny);
    std::cerr <<std::endl;
}


template <typename T>
static inline void test_ops()
{
	const size_t num = 1024;
	vector<T> v( num, as_capacity );
	for( size_t i=0; i < num; ++i )
	{
		const T tmp( gen<T>::get() );
		v.push_back( tmp );
	}
	std::cerr << "Testing " << v.size() << " of<" << typeid(T).name() << ">" << std::endl;
	for( size_t i=2; i <= num; ++i )
	{
		v[i] += v[i-1];
		v[i] -= v[i-1];
		v[i] *= v[i-1];
		v[i] /= v[i-1];
	}
	
}

YOCTO_UNIT_TEST_IMPL(z)
{
	show_values<float>("float");
	show_values<double>("double");
	test_ops<float>();
	test_ops<double>();
	test_ops< complex<float> >();
	test_ops< complex<double> >();
    
    double x = -1566;
    std::cerr << "RInt(" << x << ")=" << RInt(x) << std::endl;
    x = -245.99;
    std::cerr << "RInt(" << x << ")=" << RInt(x) << std::endl;

}
YOCTO_UNIT_TEST_DONE()


