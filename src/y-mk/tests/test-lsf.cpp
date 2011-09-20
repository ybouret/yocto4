#include "yocto/utest/run.hpp"
#include "yocto/math/fit/lsf.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/hsort.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/ipower.hpp"

using namespace yocto;
using namespace math;


namespace
{
	
	template <typename T>
	class Poly 
	{
	public:
		explicit Poly() throw() {}
		virtual ~Poly() throw() {}
		
		inline T Eval( double x, const array<T> &a )
		{
			T ans = 0;
			for( size_t i=1; i <= a.size(); ++i )
			{
				ans += a[i] * ipower(x,i-1);
			}
			return ans;
		}
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Poly);
	};
	
}

YOCTO_UNIT_TEST_IMPL(lsf)
{
	
	const size_t N = 10 + alea_less_than(291);
	vector<double> X( N, 0 );
	vector<double> Y( N, 0 );
	vector<double> Z( N, 0 );
	
	for( size_t i=1; i <= N; ++i )
	{
		X[i] = 6.14 * alea<double>();
		Y[i] = Cos( X[i] ) + 0.2 * (0.5 - alea<double>() );
		
	}
	
	{
		hsort( X, Y, __compare<double> );
		ios::ocstream fp( "lsf.txt", false );
		for( size_t i=1; i <= N; ++i )
		{
			fp("%g %g\n", X[i], Y[i] );
		}
	}
	
	fit::sample<double>     S( X, Y, Z );
	Poly<double>            P;
	fit::lsf<double>::field F( &P, & Poly<double>::Eval );
	fit::lsf<double>        LeastSquare;
	
	for( size_t nv = 1; nv <= 5; ++nv )
	{
		std::cerr << "-- nvar=" << nv << std::endl;
		vector<double> aorg(nv,0);
		vector<bool>   used(nv,true);
		if( nv > 2 && alea<double>() > 0.7 ) 
			used[ 1 + alea_less_than(nv)] = false;
		
		LeastSquare( S, F, aorg, used, 1e-7);
		{
			
			ios::ocstream fp( vformat("lsf%u.txt", unsigned(nv)), false );
			for( size_t i=1; i <= N; ++i )
			{
				fp("%g %g %g\n", X[i], Y[i], Z[i] );
			}
		}
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
