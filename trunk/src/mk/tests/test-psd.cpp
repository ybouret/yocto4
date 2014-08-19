#include "yocto/utest/run.hpp"
#include "yocto/math/fft/psd.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/sig/spike-finder.hpp"
#include "yocto/sort/heap.hpp"

#include <cstdlib>

using namespace yocto;
using namespace math;

template <typename T>
void perform_psd( const size_t p, const size_t q, const size_t K)
{
    
    
	const size_t n  = 1 << p;
	const size_t m = (1<<q);
    
	std::cerr << "[PSD n=" << n << ", m= " << m << " ]/moments=" << K << std::endl;
    
	vector<T> times(n,0);
	vector<T> data(n,0);
    
	const T dt  = T(20) / n;
    
	const T f1    = T(3);
	const T phi1  = numeric<T>::two_pi * alea<T>();
	const T f2    = T(7);
	const T phi2  = numeric<T>::two_pi * alea<T>();
	const T rho   = 0.5  + 1.5  * alea<T>();
	const T noise = 0.05 + 0.05 * alea<T>();
    
    const T offset = 2*alea<T>();
	for( size_t i=1; i < n; ++i )
	{
		const T t = times[i] = (i-1) * dt;
		data[i] = Cos( numeric<T>::two_pi * f1 * t + phi1 ) + rho * Cos( numeric<T>::two_pi * f2 * t + phi2 ) + noise * alea<T>() + offset;
	}
    
	{
		ios::ocstream fp("inp.dat",false);
		for( size_t i=1; i < n; ++i )
		{
			fp("%g %g\n", times[i], data[i] );
		}
	}
	const double df = T(1)/( (m<<1) * dt );
	const size_t ns = 10;
    
	size_t idx = 0;
	typename PSD<T>::Window  WelchWindow    = cfunctor( PSD<T>::Welch );
	typename PSD<T>::Window  BartlettWindow = cfunctor( PSD<T>::Bartlett );
	typename PSD<T>::Window  HannWindow     = cfunctor( PSD<T>::Hann );
    typename PSD<T>::Window  BlackmanWindow = cfunctor( PSD<T>::Blackman);
    typename PSD<T>::Window  NutallWindow   = cfunctor( PSD<T>::Nutall);
    
    matrix<T> psd(ns,m);
    PSD<T>::Compute( WelchWindow,    &psd[++idx][1], m, data(0), n,K);
    PSD<T>::Compute( BartlettWindow, &psd[++idx][1], m, data(0), n,K);
    PSD<T>::Compute( HannWindow,     &psd[++idx][1], m, data(0), n,K);
    PSD<T>::Compute( BlackmanWindow, &psd[++idx][1], m, data(0), n,K);
    PSD<T>::Compute( NutallWindow,   &psd[++idx][1], m, data(0), n,K);
    
    vector<T> frq(m, numeric<T>::zero );
    
	{
		const char *filename = "psd.dat";
		ios::ocstream fp( filename, false );
		for( size_t i=1; i < m; ++i )
		{
			const T f = df * (i-1);
            frq[i] = f;
			fp("%g", f);
			for( size_t j=1; j <= idx; ++j )
			{
				fp(" %g", psd[j][i] );
			}
			fp("\n");
		}
	}
    
    vector< spike_info<T> > spikes;
    const size_t spk = spike_finder<T>::detect( spikes, 5, frq, psd[1] );
    std::cerr << "detected " << spk << " spikes" << std::endl;
    for(size_t i=1; i <= spikes.size(); ++i)
    {
        std::cerr << "Spike #" << i << "@idx=" << spikes[i].idx << ": pos=" << spikes[i].pos << ", val=" << spikes[i].val << std::endl;
    }
    
    hsort(spikes, spike_info<T>::compare_by_pos);
    {
        ios::ocstream fp("spikes.dat",false);
        for(size_t i=1; i <= spikes.size(); ++i)
        {
            fp("%g %g\n", spikes[i].pos, spikes[i].val);
        }
    }
}


YOCTO_UNIT_TEST_IMPL(psd)
{
	if( argc < 4 )
		throw exception("Need p,q and K");
    
	perform_psd<double>( atol(argv[1]), atol(argv[2]), atol(argv[3]));
    
    
}
YOCTO_UNIT_TEST_DONE()