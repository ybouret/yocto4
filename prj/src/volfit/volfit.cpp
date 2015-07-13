#include "yocto/math/fit/lsf.hpp"
#include "yocto/math/io/data-set.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/exception.hpp"


using namespace yocto;
using namespace math;

namespace
{
    
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
            y[1]     = 1;    // reduced volume
            y[2]     = 1;    // reduced concentration
            double h = 1e-6; // initial step size
            odeint(diffeq,y,0,t,h,NULL);
            return y[1];
        }
        
        double fitVolume(double t, const array<double> &a )
        {
            tao::set(param,a);
            return param[4] * computeVolume(t);
        }
        
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Volume);
    };
    
}

#include "yocto/fs/vfs.hpp"

int main( int argc, char *argv[] )
{
    
    const char *prog = vfs::get_base_name(argv[0]);
    
    try
    {
        
        if(argc<4)
        {
            throw exception("usage: %s datafile #column_to_process time_limit",prog);
        }
        
        const size_t   nvar = 4;
        Volume         vol(nvar);
        vector<double> aorg(nvar,0);
        vector<double> aerr(nvar,0);
        vector<bool>   used(nvar,true);
        
        vector<double> ave_aorg(nvar,0);
        vector<double> ave_aerr(nvar,0);
        
        
        
        LeastSquares<double>          fit;
        fit.verbose = true;
        fit.fast    = true;
        
        LeastSquares<double>::Samples samples;
        
        const string fn   = argv[1];
        const size_t icol = strconv::to<size_t>(argv[2],"#columns");
        const double tmax = strconv::to<double>(argv[3],"time_limit");
        std::cerr << "Fitting column #" << icol << " of '" << fn << "'" << std::endl;
        
        
        vector<double> t,V,Vf;
        const string logfile = fn + vformat(".fit.log");
        ios::ocstream::overwrite(logfile);
        
        vector<double> torg,Vorg;
        
        for(size_t c=icol;c<=icol;++c)
        {
            std::cerr << std::endl << "-- column " << c << std::endl;
            samples.free();
            {
                data_set<double> ds;
                ds.use(1,t);
                ds.use(1+c,V);
                ios::icstream fp(fn);
                ds.load(fp);
            }
            size_t n=t.size();
            Vf.make(n,0);
            torg.make(n,0);
            Vorg.make(n,0);
            const double V1 = V[1];
            for(size_t i=1;i<=n;++i)
            {
                V[i]    = V[i]/V1;
                torg[i] = t[i];
                Vorg[i] = V[i];
            }
            
            if(tmax>0)
            {
                while(t.size()>0 && t.back() > tmax)
                {
                    t.pop_back();
                    V.pop_back();
                    Vf.pop_back();
                }
                
                n = t.size();
            }
            
            samples.append(t,V,Vf);
            samples.prepare(nvar);
            
            aorg[1] = 10;   // t0
            aorg[2] = 0.03;  // K
            aorg[3] = 0.5;  // Cosm
            aorg[4] = V[1]; // scaling
            
            if( !fit(samples,vol.fitter,aorg,used,aerr,NULL) )
            {
                throw exception("unable to fit volume");
            }
            
            std::cerr << "aorg=" << aorg << std::endl;
            std::cerr << "aerr=" << aerr << std::endl;
            std::cerr << "corr=" << samples.corr() << std::endl;
            
            //const string output = fn + vformat(".%u.fit", unsigned(c));
            const double scaling = aorg[4];
            const string output = fn + vformat(".fit%u.dat",unsigned(c));
            {
                ios::ocstream fp(output,false);
                for(size_t i=1;i<=n;++i)
                {
                    fp("%g %g %g\n", t[i], V[i]/scaling, Vf[i]/scaling );
                }
            }
            
            {
                ios::ocstream fp(logfile,true);
                fp("%s, col#%u\n", fn.c_str(), unsigned(c));
                fp("t0 = %g +/- %g\n", aorg[1], aerr[1]);
                fp("K  = %g +/- %g\n", aorg[2], aerr[2]);
                fp("Osm= %g +/- %g\n", aorg[3], aerr[3]);
                fp << "\n";
            }
            
            tao::add(ave_aorg, aorg);
            tao::add(ave_aerr, aerr);
        }
        for(size_t i=1;i<=nvar;++i)
        {
            ave_aerr[i] /= 1;
            ave_aorg[i] /= 1;
        }
        
        {
            ios::ocstream fp(logfile,true);
            fp("averages\n");
            fp("K  = %g +/- %g\n", ave_aorg[2], ave_aerr[2]);
            fp("Osm= %g +/- %g\n", ave_aorg[3], ave_aerr[3]);
        }
        
        
        
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cerr << "Unhandled Exception" << std::endl;
        return 1;
    }
    return 0;
}


