#include "yocto/utest/run.hpp"
#include "yocto/threading/server.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace threading;


namespace {
    

    class Work
    {
    public:
        int       value;
        lockable &access;
        static   double secs;
        
        Work( int v, lockable &l ) throw() : value(v), access(l)  {}
        
        ~Work() throw() {}
        
        Work( const Work &w ) throw() : value( w.value ), access( w.access ) {}
        
        void operator()(void)
        {
            {
                YOCTO_LOCK(access);
                std::cerr << "Working @" << value << "..." << std::endl;
                std::cerr.flush();
            }
            wtime::sleep(secs);
        }
        
    private:
        YOCTO_DISABLE_ASSIGN(Work);
    };
    
    double Work::secs = 0.1;
}

YOCTO_UNIT_TEST_IMPL(server)
{
    
    size_t nj = 100;
    if(argc>1)
    {
        nj = strconv::to<size_t>(argv[1],"#jobs");
    }
    
    server s;
    {
        wtime chrono;
        chrono.start();
        for(size_t i=1;i<=nj;++i)
        {
            Work w(i,s.access);
            const server::job J(w);
            s.enqueue(J);
        }
        s.flush();
        const double ell = chrono.query();
        const double tot = nj *  Work::secs;
        std::cerr << "SpeedUp=" << tot/ell << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()