#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/vfs-utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/io/data-set.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/math/fft/psd.hpp"
#include "yocto/code/hsort.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/dat/spline.hpp"

#include <iostream>

using namespace yocto;
using namespace math;

static const size_t default_size = 1024 * 1024;

static const double Chi2( size_t i,
                         const double f_star,
                         const double d_star,
                         const array<double> &t,
                         const array<double> &f)
{
    return Fabs(f[i] - f_star) + Fabs( (f[i+1]-f[i]) - (t[i+1]-t[i]) * d_star);
}

int main( int argc, char *argv[] )
{
    const char *progname = _vfs::get_base_name(argv[0]);
    _rand.wseed();
    try
    {
        
        if( argc > 1 && strcmp(argv[1],"-g")==0 )
        {
            if( argc <= 2)
                throw exception("Need Frequencies to generate data");
            vector<double> Frq;
            for( int i=2; i < argc; ++i )
            {
                Frq.push_back( strconv::to_double(argv[i],"freq"));
            }
            hsort(Frq);
            const size_t nf     = Frq.size();
            const double FrqMin = Frq[1];
            const double FrqMax = Frq[ nf ];
            const double SimMax = (10.0 + 10.0 * alea<double>())/FrqMin;
            const double SimDT  = (1.0/FrqMax) / ( 10.0 + 10.0 * alea<double>() );
            const size_t np     = 1 + ceil(SimMax/SimDT);
            std::cerr << "Simulating " << np << " points..." << std::endl;
            
            vector<double> amplitude(nf,0);
            vector<double> phase(nf,0);
            vector<double> omega(nf,0);
            for( size_t i=1; i <= nf; ++i)
            {
                omega[i]     = Frq[i] * numeric<double>::two_pi;
                amplitude[i] = 1.0 + 9.0 * alea<double>();
                phase[i]     = alea<double>() * numeric<double>::two_pi;
            }
            
            {
                ios::ocstream fp("generated.dat", false);
                for( size_t i=0; i < np; ++i )
                {
                    double ans = 0;
                    const double t   = SimDT * i;
                    for( size_t j=1; j <= nf; ++j )
                    {
                        ans += amplitude[j] * cos( t * omega[j] + phase[j] ) + 0.5 * ( alea<double>() - 0.5);
                    }
                    fp("%g %g\n", t, ans);
                }
                
                
            }
            
            return 0;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Parse Command Line
        //
        ////////////////////////////////////////////////////////////////////////
        
        if( argc <= 3)
            throw exception("usage: %s input_file #col FreqMin", progname);
        const string input_file = argv[1];
        const size_t col_index  = strconv::to_size(   argv[2], "#col" );
        double       FreqMin    = strconv::to_double( argv[3], "FreqMin");
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Load Data
        //
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "\t-- loading " << input_file << std::endl;
        vector<double> t_raw( default_size, as_capacity );
        vector<double> f_raw( default_size, as_capacity );
        {
            data_set<double> ds;
            ds.use(1,         t_raw);
            ds.use(col_index, f_raw);
            ios::icstream in( input_file );
            ds.load(in);
        }
        
        const size_t n_raw = t_raw.size();
        std::cerr << "\t-- checking" << std::endl;
        if( n_raw < 2 )
            throw exception("Not Enough Data");
        
        for(size_t i=2; i <= n_raw; ++i )
        {
            if( t_raw[i-1] >= t_raw[i] )
                throw exception("Invalid Time Line");
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Resample
        //
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "\t-- resampling" << std::endl;
        
        const size_t   n    = next_power_of_two( n_raw );
        vector<double> t( n, 0 );
        vector<double> f( n, 0 );
        
        for(size_t i=1; i<=n_raw; ++i)
        {
            t[i] = t_raw[i];
            f[i] = f_raw[i];
        }
        
        {
            //==================================================================
            //-- phase 1: find the best fitting point
            //==================================================================
            const double f_star = f_raw[n_raw];
            const double d_star = (f_raw[n_raw] - f_raw[n_raw-1])/(t_raw[n_raw]-t_raw[n_raw-1]);
            size_t best_i=1;
            double best_v=Chi2(best_i, f_star, d_star, t_raw, f_raw);
            for(size_t j=2; j < n_raw; ++j)
            {
                const double tmp = Chi2(j, f_star, d_star, t_raw, f_raw);
                if( tmp < best_v)
                {
                    best_i = j;
                    best_v = tmp;
                }
            }
            std::cerr << "best@" <<  t_raw[best_i] << std::endl;
            const size_t missing = n-n_raw;
            const double t_best  = t_raw[best_i];
            const double t_last  = t_raw[n_raw];
            
            for(size_t i=1; i <= missing; ++i )
            {
                const size_t tgt = n_raw  +i;
                const size_t src = best_i +i;
                const double del = t[src] - t_best; //!< little trick
                t[tgt] = t_last + del;
                f[tgt] = f[src];                    //!< same trick
            }
            {
                ios::ocstream fp("resampled1.dat",false);
                for(size_t i=1; i <= n; ++i) fp("%g %g\n", t[i], f[i]);
            }
        }
        
        const double dt = (t[n] - t[1]) / (n-1);
        std::cerr << "-- dt=" << dt << std::endl;
        {
            //==================================================================
            //-- phase 2: use splines for isosampling
            //==================================================================
            const spline<double>::boundary bnd(true,0);
            spline<double> s(t,f,bnd,bnd);
            const double t0 = t[1];
            for(size_t i=1;i<=n;++i)
            {
                t[i] = t0 + (i-1) * dt;
                f[i] = s(t[i]);
            }
            {
                ios::ocstream fp("resampled2.dat",false);
                for(size_t i=1; i <= n; ++i) fp("%g %g\n", t[i], f[i]);
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Evaluating PSD parameters
        //
        ////////////////////////////////////////////////////////////////////////
        //const size_t m_max = (n/2);
        
        const size_t m = n/2;
        const double M = m << 1;
        const double df = 1.0/(dt*M);
        vector<double> frq(m,0);
        vector<double> psd(m,0);
        for(size_t i=1;i<=m;++i) frq[i] = (i-1) * df;
        
        PSD<double>::Window w = cfunctor(PSD<double>::Welch);
        PSD<double>::Compute(w, psd, f, PSD_Overlap);
        {
            ios::ocstream fp("psd.dat",false);
            for(size_t i=1; i <=m; ++i) fp("%g %g\n",frq[i],psd[i]);
        }
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled exception in " << progname << std::endl;
    }
    return 1;
}
