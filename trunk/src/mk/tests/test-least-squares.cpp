#include "yocto/utest/run.hpp"
#include "yocto/math/fit/least-squares.hpp"
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

YOCTO_UNIT_TEST_IMPL(least_squares)
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
    
    typedef least_squares<double> LSF;
    typedef LSF::sample           Sample;
    typedef LSF::samples          Samples;
    typedef Sample::pointer       SamplePtr;
    diffusion     diff;
    LSF::function F( &diff, & diffusion::compute );
    
    SamplePtr S1( new  Sample(t1,x1,z1) );
    SamplePtr S2( new  Sample(t2,x2,z2) );
    
    const size_t nvar = 3;
    const size_t npar = 2;
    
    S1->prepare(nvar,npar);
    S2->prepare(nvar,npar);
    {
        S1->Gamma[1][1] = 1;
        S1->Gamma[2][3] = 1;
        std::cerr << "Gamma1=" << S1->Gamma << std::endl;
    }
    
    {
        S2->Gamma[1][2] = 1;
        S2->Gamma[2][3] = 1;
        std::cerr << "Gamma2=" << S2->Gamma << std::endl;
    }
    
    vector<double> a(3,0);
    vector<bool>   used(3,true);
    a[1] = 0.10;
    a[2] = 0.10;
    a[3] = 0;
    
    LSF lsf;
    
    Samples samples;
    samples.push_back(S1);
    samples.push_back(S2);
    
    if(argc>1)
    {
        a[3] = strconv::to<double>( argv[1], "t0");
        used[3] = false;
    }
    
    vector<double> aerr( a.size(), 0 );
    
    lsf.verbose = true;
    lsf.ftol    = 1e-12;
    lsf(F,samples,a,used,aerr);
    
    std::cerr << "aorg=" << a    << std::endl;
    std::cerr << "aerr=" << aerr << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/code/rand.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/sort/heap.hpp"
#include "yocto/ios/ocstream.hpp"

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
		
		bool ToDo(typename       least_squares<T>::function &F,
                  const typename least_squares<T>::samples  &Samples)
		{
			
			return true;
		}
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Poly);
	};
	
}

YOCTO_UNIT_TEST_IMPL(lsf_poly)
{
	
	const size_t N = 10 + alea_leq(290);
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
	
    least_squares<double>::samples Samples;
    Samples.append(X,Y,Z);
	Poly<double>            P;
    
	least_squares<double>::callback cb( &P, & Poly<double>::ToDo );
	least_squares<double>::function F(  &P, & Poly<double>::Eval );
    least_squares<double> LeastSquare;
	
    
	for( size_t nv = 1; nv <= 5; ++nv )
	{
		std::cerr << "-- nvar=" << nv << std::endl;
		vector<double> aorg(nv,0);
		vector<bool>   used(nv,true);
		vector<double> aerr(nv,0);
		
		if( nv > 2 && alea<double>() > 0.7 )
			used[ 1 + alea_lt(nv) ] = false;
		
		LeastSquare.ftol = 1e-7;
		if(  LeastSquare(F, Samples, aorg, used, aerr , NULL)  != least_squares_failure )
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

namespace
{
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
        
        bool ToDo(typename       least_squares<T>::function &F,
                  const typename least_squares<T>::samples  &Samples)
        {
            const array<T> &a = Samples[1]->u;
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
    
}

YOCTO_UNIT_TEST_IMPL(lsf_gauss)
{
	const size_t   N = 10 + alea_leq(1000);
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
    
    least_squares<double>::samples Samples;
    Samples.append(X,Y,Z);
    
	Gauss<double>                   G;
	least_squares<double>::function F( &G, & Gauss<double>::Eval );
    least_squares<double>::callback cb( &G, & Gauss<double>::ToDo );
	least_squares<double>           LeastSquare;
	a[1] = 1;
	a[2] = 0.25;
	a[3] = 0.01;
	
	a[4] = 1;
	a[5] = 0.75;
	a[6] = 0.01;
	
	
	LeastSquare.h = 1e-4;
	LeastSquare.verbose = true;
    
	if( LeastSquare( F, Samples, a, used, aerr, &cb) != least_squares_failure )
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


YOCTO_UNIT_TEST_IMPL(pade)
{
	const size_t   N = 10 + alea_leq(1000);
	vector<double> X( N, 0 );
	vector<double> Y( N, 0 );
	vector<double> Z( N, 0 );
    
    for(size_t i=1;i<=N;++i)
    {
        X[i] = double(i-1)/(N-1);
        Y[i] = cos(3*X[i]);
    }
    
    size_t p=0;
    size_t q=0;
    if( argc > 1 )
        p = strconv::to_size(argv[1],"p");
        
    if( argc > 2 )
        q = strconv::to_size(argv[2],"q");
    
    least_squares<double>::sample Sample(X,Y,Z);
    vector<double> P(p,0);
    vector<double> Q(q,0);
    
    Sample.Pade(P, Q);
    
    std::cerr << "P=" << P << std::endl;
    std::cerr << "Q=" << Q << std::endl;
    
    ios::ocstream fp("pade.dat",false);
    for( size_t i=1; i <= N; ++i )
    {
        fp("%g %g %g\n", X[i], Y[i], Z[i] );
    }
    
}
YOCTO_UNIT_TEST_DONE()

