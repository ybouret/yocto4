#include "../wtime.hpp"
#include "../error.hpp"
#include "../mutex.hpp"
#include "../exception.hpp"
#include "../rand32.hpp"
#include "../team.hpp"
#include "../c_array.hpp"
#include "../list.hpp"
#include "../pool.hpp"
#include "../stats.hpp"


struct ThreadData
{
    Mutex *mutex;
};

static void ThreadProc(void *args) throw()
{
    assert(args);
    ThreadData &data = *(ThreadData *)args;
    PYCK_LOCK( *data.mutex );
    std::cerr << "From Thread" << std::endl;
    std::cerr.flush();
}

class Task : public Runnable
{
public:
    const int id;
    explicit Task( Mutex &m, int n) throw() : Runnable(m), id( n )  {}
    virtual ~Task() throw() {}
    
private:
    Task( const Task & );
    Task &operator=( const Task ) throw();
    
    virtual void run() throw()
    {
        PYCK_LOCK(mutex);
        std::cerr << "In Task #" << id << std::endl;
        std::cerr.flush();
    }
    
};

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
        {
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
        }
        
        ////////////////////////////////////////////////////////////////////////
        // C_Array
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing C_Array" << std::endl;
        {
            C_Array<uint32_t> seeds(10);
            for( size_t i=0; i < seeds.size; ++i ) seeds[i] = WallTime::Seed();
            std::cerr << "seeds=" << seeds << std::endl;
            double ave=0,sig=0;
            Stats::AverageAndStdErr(ave, sig, seeds);
            std::cerr << "ave=" << ave << std::endl;
            std::cerr << "sig=" << sig << std::endl;
        }
        
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
            double sum_x  = 0;
            double sum_x2 = 0;
            chrono.start();
            const size_t N = 1000000;
            for( size_t j=0; j < N; ++j )
            {
                const double x = R.get<double>();
                sum_x  += x;
                sum_x2 += x*x;
            }
            const double ell = chrono.query();
            const double ave = sum_x / N;
            const double sig = (sum_x2 - 2*ave*sum_x + N * ave*ave )/(N-1);
            const double spd = (N/ell) * 1e-6;
            std::cerr << "Speed" << i << "=" << spd << " M/s, Ave=" << ave << ", Sig=" << sig << std::endl;
        }
        std::cerr << std::endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Testing Threads
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Threads" << std::endl;
        ThreadData data;
        data.mutex = &mtx;
        Thread thr1( ThreadProc, &data);
        Thread thr2( ThreadProc, &data);
        thr1.join();
        thr2.join();
        std::cerr << std::endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Testing Runnable
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Runnable" << std::endl;
        Task task1( mtx, 63 );
        Task task2( mtx, 78 );
        
        task2.start();
        task1.start();
        
        task1.join();
        task2.join();
        std::cerr << std::endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Testing Team
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Team" << std::endl;
        const size_t nt = 4;
        C_Array<int> arr(nt);
        Team<Task>   team(nt);
        
        for( size_t j=0; j < 2; ++j )
        {
            std::cerr << "Start" << std::endl;
            for( size_t i=0; i<nt;++i)
                arr[i] = r1.leq(100);
            
            for(size_t i=0;i<nt;++i)
            {
                team.enqueue(arr[i]);
            }
            team.finish();
            std::cerr << "done" << std::endl << std::endl;
        }
        
        ////////////////////////////////////////////////////////////////////////
        // Testing Team
        ////////////////////////////////////////////////////////////////////////
        std::cerr << "-- Testing Team::Split" << std::endl;
        const size_t workload = 20;
        for( size_t jsize=1; jsize <=8; ++jsize )
        {
            std::cerr << "\t#JOBS=" << jsize << std::endl;
            for(size_t jrank=0; jrank<jsize; ++jrank )
            {
                size_t offset=0, length=0;
                TeamBalance(workload,offset,length,jrank,jsize);
                std::cerr << "\t\t" << jsize << "." << jrank << ": " << offset << "+" << length << std::endl;
            }
            std::cerr << std::endl;
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

