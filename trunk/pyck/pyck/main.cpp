#include "./wtime.hpp"
#include "./error.hpp"
#include "./mutex.hpp"
#include "./exception.hpp"

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

