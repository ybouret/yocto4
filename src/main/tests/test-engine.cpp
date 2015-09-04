#include "yocto/threading/engine.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/code/rand32.hpp"

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
            rand32_kiss r;
            r.wseed();
            wtime chrono;
            chrono.start();
            while(chrono.query()<secs)
            {
                (void)r.next();
            }
        }

    private:
        YOCTO_DISABLE_ASSIGN(Work);
    };

    double Work::secs = 0.4;
}

YOCTO_UNIT_TEST_IMPL(engine)
{
    size_t nj = 1;
    if(argc>1)
    {
        nj = strconv::to<size_t>(argv[1],"#jobs");
    }

    engine Q;
    std::cerr << "Enqueue " << nj << " jobs..." << std::endl;
    {
        for(size_t i=1;i<=nj;++i)
        {
            Work w(i);
            const engine::job J(w);
            Q.enqueue(J);
        }
        {
            YOCTO_LOCK(Q.access);
            std::cerr << "\t---- Program is waiting ----" << std::endl;
        }
        wtime::sleep(2);
    }
    
}
YOCTO_UNIT_TEST_DONE()
