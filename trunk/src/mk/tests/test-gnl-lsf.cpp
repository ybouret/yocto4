#include "yocto/utest/run.hpp"
#include "yocto/math/fit/gnl-lsf.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;


namespace
{
	struct diffusion
	{

		double compute( double t, const array<double> &a )
		{
			const double slope = a[1];
			const double t0    = a[2];
			return sqrt( slope*(t-t0) );
		}

	};
}

YOCTO_UNIT_TEST_IMPL(gnl)
{


	const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
	const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};

	const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
	const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};

	const lw_array<double> t1( (double*)_t1, sizeof(_t1)/sizeof(_t1[0]) );
	const lw_array<double> x1( (double*)_x1, sizeof(_x1)/sizeof(_x1[0]) );
	const lw_array<double> t2( (double*)_t2, sizeof(_t2)/sizeof(_t2[0]) );
	const lw_array<double> x2( (double*)_x2, sizeof(_x2)/sizeof(_x2[0]) );

	vector<double> z1(t1.size(),0);
	vector<double> z2(t2.size(),0);

}
YOCTO_UNIT_TEST_DONE()
