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
		
		
		bool ToDo(const fit::sample<T>              &s, 
				  const typename fit::lsf<T>::field &f,
				  const array<T>                    &a )
		{
			
			return true;
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
		ios::ocstream fp( "lsf.dat", false );
		for( size_t i=1; i <= N; ++i )
		{
			fp("%g %g\n", X[i], Y[i] );
		}
	}
	
	fit::sample<double>     S( X, Y, Z );
	Poly<double>            P;
	fit::lsf<double>::callback cb( &P, & Poly<double>::ToDo );
	fit::lsf<double>::field F( &P, & Poly<double>::Eval );
	fit::lsf<double>        LeastSquare;
	
	for( size_t nv = 1; nv <= 5; ++nv )
	{
		std::cerr << "-- nvar=" << nv << std::endl;
		vector<double> aorg(nv,0);
		vector<bool>   used(nv,true);
		vector<double> aerr(nv,0);
		
		if( nv > 2 && alea<double>() > 0.7 ) 
			used[ 1 + alea_less_than(nv)] = false;
		
		LeastSquare.ftol = 1e-7;
		LeastSquare( S, F, aorg, used, aerr , &cb);
		
		if( S.status == fit::success )
		{
			for( size_t i=1; i <= nv; ++i )
			{
				std::cerr << "a[" << i << "]=" << aorg[i] << " +/- " << aerr[i] << std::endl;
			}
			ios::ocstream fp( vformat("lsf.%u.dat", unsigned(nv)), false );
			for( size_t i=1; i <= N; ++i )
			{
				fp("%g %g %g\n", X[i], Y[i], Z[i] );
			}
		}
	}
	
	
}
YOCTO_UNIT_TEST_DONE()


static inline double gaussian( double x, double mu, double sig )
{
	const double dx = x-mu;
	return Exp( - (dx*dx)/(sig+sig) );
}

template <typename T>
class Gauss 
{
public:
	
	Gauss() throw()
	{
	}
	
	~Gauss() throw()
	{
	}
	
	T Eval( T x, const array<T> &a )
	{
		const size_t nexp = a.size()/3;
		size_t i=1;
		T ans(0);
		for( size_t j=1; j <= nexp; ++j )
		{
			const T A   = a[i++];
			const T mu  = a[i++];
			const T sig = a[i++];
			ans += A * gaussian(x, mu, sig);
		}
		return ans;
	}
	
	bool ToDo(const fit::sample<T>              &s, 
			  const typename fit::lsf<T>::field &f,
			  const array<T>                    &a )
	{
		const size_t nexp = a.size()/3;
		size_t i=1;
		for( size_t j=1; j <= nexp; ++j )
		{
			const T A   = a[i++];
			const T mu  = a[i++];
			const T sig = a[i++];
			if(j>1) std::cerr << "+";
			std::cerr << "(" << A << ")*G(" << mu << "," << sig << ")"; 
		}
		std::cerr << std::endl;
		return true;
	}
};

YOCTO_UNIT_TEST_IMPL(lsf2)
{
	const size_t   N = 10 + alea_less_than(1000);
	vector<double> X( N, 0 );
	vector<double> Y( N, 0 );
	vector<double> Z( N, 0 );
	
	const double x1 = 0.21;
	const double s1 = 0.02;
	const double a1 = 1.1;
	
	const double x2 = 0.78;
	const double s2 = 0.01;
	const double a2 = 2.3;
	
	for( size_t i=1; i <= N; ++i )
	{
		X[i] = double(i-1)/(N-1);
		Y[i] = a1 * gaussian( X[i], x1, s1) + a2 * gaussian( X[i], x2, s2 ) + 0.2 * ( 0.5 - alea<double>());
		
	}
	
	{
		ios::ocstream fp( "mexp.dat", false );
		for( size_t i=1; i <= N; ++i )
		{
			fp("%g %g\n", X[i], Y[i] );
		}
	}
	
	vector<double> a(6,0);
	vector<bool>   used(6,true);
	vector<double> aerr(6,0);
	
	fit::sample<double>     S( X, Y, Z );
	Gauss<double>           G;
	fit::lsf<double>::field F( &G, & Gauss<double>::Eval );
	fit::lsf<double>        LeastSquare;
	a[1] = 1;
	a[2] = 0.25;
	a[3] = 0.01;
	
	a[4] = 1;
	a[5] = 0.75;
	a[6] = 0.01;
	
	fit::lsf<double>::callback cb( &G, & Gauss<double>::ToDo );
	
	LeastSquare.h = 1e-4;
	LeastSquare( S, F, a, used, aerr, &cb);
	if( S.status == fit::success )
	{
		for( size_t i=1; i <= 6; ++i )
		{
			std::cerr << "a[" << i << "]=" << a[i] << " +/- " << aerr[i] << std::endl;
		}
		ios::ocstream fp( "mexp.6.dat", false );
		for( size_t i=1; i <= N; ++i )
		{
			fp("%g %g %g\n", X[i], Y[i], Z[i] );
		}
	}
}
YOCTO_UNIT_TEST_DONE()

