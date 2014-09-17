#include "yocto/utest/run.hpp"
#include "yocto/math/fit/lsf.hpp"

#include "yocto/math/io/data-set.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"

#include "yocto/math/kernel/tao.hpp"

using namespace yocto;
using namespace math;


namespace {
 
    class Volume
    {
    public:
        ode::driverCK<double>::type    odeint;
        vector<double>                 param;
        ode::Field<double>::Equation   diffeq;
        LeastSquares<double>::Function fitter;
        vector<double>                 y;
        
        explicit Volume() :
        odeint(1e-7),
        param(2,0),
        diffeq( this , & Volume::derivs    ),
        fitter( this,  & Volume::fitVolume ),
        y(1,0)
        {
            odeint.start(y.size());
        }
        
        virtual ~Volume() throw()
        {
        }
        
        void derivs( array<double> &dYdt, double t, const array<double> &Y )
        {
            const double t0 = param[1];
            const double a  = param[2];
            if(t<=t0)
            {
                dYdt[1] = 0;
            }
            else
            {
                dYdt[1] = -a * Y[1];
            }
        }
        
        double computeVolume(double t)
        {
            y[1]     = 1;
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
    size_t icol = 2;
    
    if(argc>1)
    {
        // read data
        vector<double> t;
        vector<double> V;
        {
            data_set<double> ds;
            ds.use(1, t);
            ds.use(icol,V);
            ios::icstream fp(argv[1]);
            ds.load(fp);
        }
        
        const size_t   N = t.size();
        vector<double> Vf(N,0);
        
        Volume vol;
        
        
        
        const size_t   nvar = 2;
        vector<double> aorg(nvar,0);
        vector<double> aerr(nvar,0);
        vector<bool>   used(nvar,true);
        
        
        LeastSquares<double>          fit;
        LeastSquares<double>::Samples samples;
        
        samples.append(t,V,Vf);
        samples.prepare(nvar);
        
        
        
        aorg[1] = 25;
        aorg[2] = 0.1;
        
        fit.verbose = true;
        
        if( !fit(samples,vol.fitter,aorg,used,aerr,NULL) )
        {
            throw exception("unable to fit volume");
        }
        
        {
            ios::ocstream fp("outvol.dat",false);
            for(size_t i=1;i<=N;++i)
            {
                fp("%g %g %g\n", t[i], V[i], Vf[i]);
            }
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
