#include "./wtime.hpp"
#include "./error.hpp"
#include "./mutex.hpp"
#include "./exception.hpp"
#include "./rand32.hpp"

#include <iostream>

#define __EXCP(CODE) do { try { CODE; } catch( const std::exception &e ) { std::cerr << e.what() << std::endl; } } while(false)

int main(int argc, char *argv[])
{
    try
    {
        ////////////////////////////////////////////////////////////////////////
        // Wall Time
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing WallTime" << std::endl;
        WallTime chrono;
        chrono.start();
        
        std::cerr << std::hex;
        for( size_t i=0; i < 10; ++i )
        {
            std::cerr << WallTime::Ticks() << " : " << WallTime::Seed() << std::endl;
        }
        std::cerr << std::dec;
        
        const double ell = chrono.query();
        std::cerr << "Ellapsed: " << ell << " seconds" << std::endl;
        
        std::cerr << std::endl;
        
        
        ////////////////////////////////////////////////////////////////////////
        // Errors
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Errors" << std::endl;
        for( int i=0; i < 10; ++i )
        {
            char buffer[80];
            SystemFormatError(i, buffer, sizeof(buffer));
            std::cerr << "Err" << i << ": " << buffer << std::endl;
        }
        std::cerr << std::endl;
        
        
        ////////////////////////////////////////////////////////////////////////
        // Mutex
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Mutex" << std::endl;
        Mutex mtx;
        {
            PYCK_LOCK(mtx);
            {
                PYCK_LOCK(mtx);
            }
        }
        std::cerr << std::endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Testing Exception
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Exceptions" << std::endl;
        __EXCP( throw Exception("Hello, World @line=%d", __LINE__) );
        __EXCP( throw Exception(0,"No Error") );
        std::cerr << std::endl;
        
        
        ////////////////////////////////////////////////////////////////////////
        // Testing Random
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Random" << std::endl;
        {
            Rand32 r32;
            r32.test();
        }
        rand32::mwc   r1;
        rand32::shr3  r2;
        rand32::cong  r3;
        rand32::fib   r4;
        rand32::kiss  r5;
        rand32::lfib4 r6;
        rand32::swb   r7;
        UniformGenerator *r[] = { &r1, &r2, &r3, &r4, &r5, &r6, &r7 };
        for( size_t i=0; i < sizeof(r)/sizeof(r[0]); ++i)
        {
            UniformGenerator &R = *r[i];
            R.seed( WallTime::Seed() );
            double sum = 0;
            chrono.start();
            const size_t N = 1000000;
            for( size_t j=0; j < N; ++j )
            {
                sum += R.get<double>();
            }
            const double ell = chrono.query();
            const double ave = sum /= N;
            const double spd = (N/ell) * 1e-6;
            std::cerr << "Speed=" << spd << " M/s, Ave" << i << "=" << ave << std::endl;
        }
        
        return 0;
    }
    catch( const std::exception &e )
    {
        std::cerr << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled Exception!" << std::endl;
    }
	return 1;
}

