#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/io/data-set.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/math/fft/psd.hpp"
#include "yocto/sort/heap.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/dat/spline.hpp"
#include "yocto/math/alg/spike.hpp"

#include <iostream>

using namespace yocto;
using namespace math;

static const size_t default_size = 1024 * 1024;

#if 0
static const double Chi2( size_t i,
                         const double f_star,
                         const double d_star,
                         const array<double> &t,
                         const array<double> &f)
{
    return Fabs(f[i] - f_star) + Fabs( (f[i+1]-f[i]) - (t[i+1]-t[i]) * d_star);
}
#endif

struct time_selector
{
    double tmin,tmax;
    bool   accept( double t ) const throw()
    {
        return t >= tmin && t <= tmax;
    }
};

int main( int argc, char *argv[] )
{
    const char *progname = vfs::get_base_name(argv[0]);
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
            const double offset = 5 * alea<double>();
            {
                ios::ocstream fp("generated.dat", false);
                for( size_t i=0; i < np; ++i )
                {
                    double ans = 0;
                    const double t   = SimDT * i;
                    for( size_t j=1; j <= nf; ++j )
                    {
                        ans += amplitude[j] * cos( t * omega[j] + phase[j] );
                    }
                    ans +=  0.5 * ( alea<double>() - 0.5) + offset;
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
        
        if( argc <= 5)
            throw exception("usage: %s input_file #col K tmin tmax", progname);
        int iarg = 1;
        const string  input_file = argv[iarg++];
        const size_t  col_index  = strconv::to_size( argv[iarg++], "#col" );
        const size_t  K          = strconv::to_size( argv[iarg++], "K" );
        time_selector ts = { 0, 0};
        ts.tmin          = strconv::to_double( argv[iarg++], "tmin");
        ts.tmax          = strconv::to_double( argv[iarg++], "tmax");
        if( ts.tmax < ts.tmin )
            throw exception("invalid time range !!!");
        
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
            data_set<double>::callback cb( &ts, & time_selector:: accept );
            ds.use(1,         t_raw);
            ds.use(col_index, f_raw);
            ios::icstream in( input_file );
            ds.load(in,&cb); //!< load with time selection
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
        std::cerr << "\t-- resampling " << n_raw << " data" << std::endl;
        
        const size_t   n    = n_raw;
        vector<double> t( n, 0 );
        vector<double> f( n, 0 );
        
        
        const double dt = (t_raw[n] - t_raw[1]) / (n-1);
        std::cerr << "-- dt=" << dt << std::endl;
        {
            //==================================================================
            //-- phase 2: use splines for isosampling
            //==================================================================
            spline1D<double> s(spline_natural,t_raw,f_raw);
            const double t0 = t_raw[1];
            for(size_t i=1;i<=n;++i)
            {
                t[i] = t0 + (i-1) * dt;
                f[i] = s(t[i]);
            }
            {
                ios::ocstream fp("resampled.dat",false);
                for(size_t i=1; i <= n; ++i) fp("%g %g\n", t[i], f[i]);
            }
            {
                ios::ocstream fp("original.dat",false);
                for(size_t i=1; i <= n; ++i) fp("%g %g\n", t_raw[i], f_raw[i]);
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Evaluating PSD parameters
        //
        ////////////////////////////////////////////////////////////////////////
        const size_t m_max = next_power_of_two(n)/4;
        
        const size_t m  = m_max;
        const double M  = m << 1;
        const double df = 1.0/(dt*M);
        vector<double> frq(m,0);
        vector<double> psd(m,0);
        for(size_t i=1;i<=m;++i) frq[i] = (i-1) * df;
        
        PSD<double>::Window w = cfunctor(PSD<double>::Welch);
        PSD<double>::Compute(w, psd, f, K);
        {
            ios::ocstream fp("psd.dat",false);
            for(size_t i=1; i <=m; ++i) fp("%g %g\n",frq[i],psd[i]);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Find main spikes
        //
        ////////////////////////////////////////////////////////////////////////
        vector< spike::pointer > spikes;
        spike::detect(spikes,psd);
        const size_t ns = spikes.size();
        hsort(spikes, spike::compare_by_position);
        {
            ios::ocstream fp("spikes.dat",false);
            for(size_t i=1; i <= ns; ++i )
            {
                std::cerr << "Spike #" << i << "@idx=" << spikes[i]->position
                << ": pos=" << frq[spikes[i]->position] << ", val=" << spikes[i]->value << std::endl;
                fp("%g %g\n", frq[spikes[i]->position], spikes[i]->value);
            }
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
