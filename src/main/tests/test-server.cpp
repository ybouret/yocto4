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
        static   double secs;
        
        Work( int v ) throw() : value(v)  {}
        
        ~Work() throw() {}
        
        Work( const Work &w ) throw() : value( w.value ){}
        
        void operator()(lockable &access)
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

    std::cerr << "sizeof(vslot) =" << sizeof(vslot)  << std::endl;
    std::cerr << "sizeof(thread)=" << sizeof(thread) << std::endl;

    server s;
    Work   w(0);
    const server::job     J(w);
    const server::task_id tid = s.enqueue(J);
    {
        YOCTO_LOCK(s.access);
        std::cerr << "task_id=" << tid << std::endl;
    }

#if 0
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
#endif
}
YOCTO_UNIT_TEST_DONE()
