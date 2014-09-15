#include "yocto/utest/run.hpp"
#include "yocto/math/fit/gnl-lsf.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/string/conv.hpp"

#include "yocto/ios/ocstream.hpp"


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


static inline void save(const char *fn, const array<double> &x, const array<double> &y)
{
    ios::ocstream fp(fn,false);
    for(size_t i=1;i<=x.size();++i)
    {
        fp("%g %g\n", x[i], y[i]);
    }
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

    save("d1.dat",t1,x1);
    save("d2.dat",t2,x2);
    
    
	vector<double> z1(t1.size(),0);
	vector<double> z2(t2.size(),0);

	typedef LeastSquares<double> LS;
	typedef LS::Sample  Sample;
	typedef LS::Samples Samples;

	Samples samples;
	Sample &s1 = samples.append(t1,x1,z1);

	diffusion xp;

	LS::Function F(&xp, &diffusion::compute);

	vector<double> a(3,0);

	a[1] = 0.15;
	a[2] = -150;

	s1.prepare(2,3);
	s1.Gamma[1][1] = 1;
	s1.Gamma[2][2] = 1;
	derivative<double> drvs;
    
	std::cerr << "D1    = " << s1.compute_D(F,a) << std::endl;
	std::cerr << "D1    = " << s1.compute_D(F,a,drvs,1e-4) << std::endl;
	std::cerr << "a     = " << a << std::endl;
	std::cerr << "u     = " << s1.u << std::endl;
	std::cerr << "beta  = " << s1.beta  << std::endl;
	std::cerr << "alpha = " << s1.alpha << std::endl;
	std::cerr << "Gamma = " << s1.Gamma << std::endl;
	std::cerr << "__ag  = " << s1.__ag  << std::endl;
	std::cerr << "Alpha = " << s1.Alpha  << std::endl;

    std::cerr << std::endl;
    
	vector<double> Beta( a.size(), 0);
	matrix<double> Alpha( a.size(), a.size() );
    std::cerr << "LAMBDA_MIN_POW10=" << LS::LAMBDA_MIN_POW10 << std::endl;
    std::cerr << "LAMBDA_MAX_POW10=" << LS::LAMBDA_MAX_POW10 << std::endl;

	s1.collect(Alpha,Beta);
	std::cerr << "Alpha = " << Alpha << std::endl;
	std::cerr << "Beta  = "  << Beta  << std::endl;

	LS fit;
	vector<bool> used(a.size(),true);
	used[3] = false;
	vector<double> aerr(a.size(),0);

	fit(samples,F,a,used,aerr);

    save("f1.dat",t1,z1);
    

}
YOCTO_UNIT_TEST_DONE()


#include "yocto/code/ipower.hpp"

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
		
#if 0
		bool ToDo(typename       least_squares<T>::function &F,
                  const typename least_squares<T>::samples  &Samples)
		{
			
			return true;
		}
#endif
        
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Poly);
	};
	
}

#include "yocto/code/rand.hpp"
#include "yocto/sort/quick.hpp"

YOCTO_UNIT_TEST_IMPL(fit_poly)
{
	
	const size_t   N = 10 + alea_leq(290);
	vector<double> X( N, 0 );
	vector<double> Y( N, 0 );
	vector<double> Z( N, 0 );
	
	for( size_t i=1; i <= N; ++i )
	{
		X[i] = 6.14 * alea<double>();
		Y[i] = Cos( X[i] ) + 0.2 * (0.5 - alea<double>() );
		
	}
	
	{
		co_qsort(X,Y);
		save("poly.dat",X,Y);
	}
	
    LeastSquares<double>::Samples samples;
    samples.append(X,Y,Z);
	Poly<double> P;
    
	//least_squares<double>::callback cb( &P, & Poly<double>::ToDo );
	LeastSquares<double>::Function F(  &P, & Poly<double>::Eval );
    LeastSquares<double> Fit;
	
    
	for( size_t nv = 1; nv <= 5; ++nv )
	{
		std::cerr << "-- nvar=" << nv << std::endl;
		vector<double> aorg(nv,0);
		vector<bool>   used(nv,true);
		vector<double> aerr(nv,0);
		
		if( nv > 2 && alea<double>() > 0.7 )
        {
			//used[ 1 + alea_lt(nv) ] = false;
        }
        
        samples.prepare(nv);
		Fit(samples, F, aorg, used, aerr );
		
#if 0
        //LeastSquare.ftol = 1e-7;
		if(    != least_squares_failure )
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
#endif
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
