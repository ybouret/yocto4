#include "yocto/utest/run.hpp"
#include "yocto/math/fit/lsf.hpp"

#include "yocto/math/io/data-set.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;


namespace {
    
#if defined(_MSC_VER)
#pragma warning ( disable : 4355 )
#endif
	class Volume
	{
	public:
		ode::driverCK<double>::type    odeint;
		vector<double>                 param;
		ode::Field<double>::Equation   diffeq;
		LeastSquares<double>::Function fitter;
		vector<double>                 y;
        
		explicit Volume(const size_t npar) :
		odeint(1e-7),
        param(npar,0),
        diffeq( this , & Volume::derivs    ),
        fitter( this,  & Volume::fitVolume ),
        y(2,0)
		{
			odeint.start(y.size());
		}
        
		virtual ~Volume() throw()
		{
		}
        
		void derivs( array<double> &dYdt, double t, const array<double> &Y )
		{
			const double t0   = param[1];
			const double K    = param[2];
			const double Cout = param[3];
            double       V    = Y[1];
            double       Cs   = Y[2];
			if(t<=t0)
			{
				dYdt[1] = 0;
                dYdt[2] = 0;
			}
			else
			{
                // dVdt
				dYdt[1] = K * (Cs-Cout);
                dYdt[2] = - Cs * dYdt[1] / V;
			}
		}
        
		double computeVolume(double t)
		{
			y[1]     = 1; // reduced volume
            y[2]     = 1; // reduced concentration
			double h = 1e-6;
			odeint(diffeq,y,0,t,h,NULL);
			return y[1];
		}
        
		double fitVolume(double t, const array<double> &a )
		{
			//std::cerr << "fitVolume(" << t << "," << a << ")" << std::endl;
			tao::set(param,a);
			return computeVolume(t);
		}
        
        
        
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Volume);
	};
    
}


YOCTO_UNIT_TEST_IMPL(fit_vol)
{
	
    
	if(argc>1)
	{
		size_t icol = 1;
		if(argc>2)
			icol = strconv:: to_size(argv[2],"column index");
        
       
        
		// read data
		vector<double> t;
		vector<double> V;
		{
			data_set<double> ds;
			ds.use(1, t);
			ds.use(icol+1,V);
			ios::icstream fp(argv[1]);
			ds.load(fp);
		}
        
		const size_t   N = t.size();
		vector<double> Vf(N,0);
        
        
        
        
        
		const size_t   nvar = 3;
		Volume         vol(nvar);
		vector<double> aorg(nvar,0);
		vector<double> aerr(nvar,0);
		vector<bool>   used(nvar,true);
        
        
		LeastSquares<double>          fit;
        if(argc>3&&0==strcmp(argv[3],"fast"))
        {
            fit.fast = true;
        }
		LeastSquares<double>::Samples samples;
        
		samples.append(t,V,Vf);
		samples.prepare(nvar);
        
        
        
		aorg[1] = 25;   // t0
		aorg[2] = 0.1;  // K
		aorg[3] = 1.2;  // Cs
        
        
        aorg[1] = 23;   // t0
        aorg[2] = 0.1;  // K
        aorg[3] = 2.0;  // Cosm

        
		fit.verbose = true;
        
		if( !fit(samples,vol.fitter,aorg,used,aerr,NULL) )
		{
			throw exception("unable to fit volume");
		}
        
        std::cerr << std::endl;
        std::cerr << "Fit Results: " << std::endl;
        std::cerr << "aorg=" << aorg << std::endl;
        std::cerr << "aerr=" << aerr << std::endl;
        std::cerr << "corr=" << samples.corr() << std::endl;
        
		{
			ios::ocstream fp(vformat("outvol%u.dat",unsigned(icol)),false);
			for(size_t i=1;i<=N;++i)
			{
				fp("%g %g %g\n", t[i], V[i], Vf[i]);
			}
		}
	}
    
}
YOCTO_UNIT_TEST_DONE()
