#include "yocto/utest/run.hpp"
#include "yocto/threading/server.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sys/wtime.hpp"

using namespace yocto;
using namespace threading;


namespace {
    
    class Work
    {
    public:
        int       value;
        lockable &access;
        
        Work( int v, lockable &l ) throw() : value(v), access(l)  {}
        
        ~Work() throw() {}
        
        Work( const Work &w ) throw() : value( w.value ), access( w.access ) {}
        
        void operator()(void)
        {
            double secs=0;
            {
                YOCTO_LOCK(access);
                std::cerr << "Working @" << value << "..." << std::endl;
                std::cerr.flush();
                secs = 0.2 * alea<double>();
            }
            wtime::sleep(secs);
        }
        
    private:
        YOCTO_DISABLE_ASSIGN(Work);
    };
}

YOCTO_UNIT_TEST_IMPL(server)
{
    
    
    server s;
    {
        
        for(int i=1;i<=100;++i)
        {
            Work w(i,s.access);
            const server::job J(w);
            s.enqueue(J);
        }
        s.flush();
    }
    
}
YOCTO_UNIT_TEST_DONE()