#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/vfs-utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/io/data-set.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/dat/spline.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/math/fft/psd.hpp"
#include "yocto/code/hsort.hpp"
#include "yocto/code/rand.hpp"

#include <iostream>

using namespace yocto;
using namespace math;

static const size_t default_size = 1024 * 1024;


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
                        ans += amplitude[j] * cos( t * omega[j] + phase[j] );
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
        
        const size_t n    = next_power_of_two( t_raw.size() );
        const double Tmax = t_raw[n_raw] - t_raw[1];
        const double Tmin = Tmax / (n-1);
        vector<double> t(n,0);
        vector<double> f(n,0);
        {
            const spline<double>::boundary blo( true, 0 );
            const spline<double>::boundary bhi( true, 0 );
            spline<double> s_raw(t_raw,f_raw,blo,bhi);
            for( size_t i=1; i <= n; ++i )
            {
                t[i] = (i-1) * Tmin + t_raw[1];
                f[i] = s_raw( t[i] );
            }
        }
        
        std::cerr << "\t-- #raw=" << n_raw << " => " << n << " samples" << std::endl;
        {
            ios::ocstream fp("resampled.dat", false);
            for(size_t i=1; i <= n; ++i ) fp("%g %g\n", t[i], f[i]);
        }
        std::cerr << "\t-- Tmax    = " << Tmax << "s" << std::endl;
        std::cerr << "\t-- Tmin    = " << Tmin << "s" << std::endl;
        
        const double FreqMaxRaw = 1.0 / Tmin;
        const double FreqMinRaw = 1.0 / Tmax;
        std::cerr << "\t-- FreqMaxRaw = " << FreqMaxRaw << " Hz" << std::endl;
        std::cerr << "\t-- FreqMinRaw = " << FreqMinRaw << " Hz" << std::endl;
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Power Spectrum Density
        //
        ////////////////////////////////////////////////////////////////////////
        const size_t M_max      = n;
        const size_t m_max      = M_max >> 1;
        const double TmaxPSD    = m_max * Tmin;
        const double FreqMinPSD = 1.0 / TmaxPSD; //-- 2*FreqMinRaw
        std::cerr << "\t-- FreqMinPSD = " << FreqMinPSD << " Hz" << std::endl;
        std::cerr << "\t-- m_max      = " << m_max << std::endl;
        FreqMin = max_of( FreqMin, FreqMinPSD);
        
        size_t        m = min_of( m_max, next_power_of_two<size_t>( 1.0/ (Tmin*FreqMin)) / 2 );
        const double df = 1.0 / ( Tmin * (m<<1) );
        std::cerr << "m=" << m << std::endl;
        std::cerr << "df=" << df << std::endl;
        vector<double> psd(m,0);
        vector<double> frq(m,0);
        for( size_t i=1; i <= m; ++i )
        {
            frq[i] = (i-1) * df;
        }
        PSD<double>::Window w = cfunctor( PSD<double>::Welch );
        PSD<double>::Compute( w, psd, f, PSD_Overlap);
        {
            ios::ocstream fp("psd.dat",false);
            for(size_t i=1; i <= m; ++i) fp("%g %g\n", frq[i], psd[i]);
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
