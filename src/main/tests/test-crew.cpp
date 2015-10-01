#include "yocto/threading/crew.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand32.hpp"
#include "yocto/parallel/basic.hpp"
#include "yocto/sys/timings.hpp"

using namespace yocto;
using namespace threading;

namespace
{

    class Worker
    {

    public:
        double        total;
        static size_t Count;
        static size_t Loops;
        static const size_t nTab = 1024;
        static double tab[nTab];

        explicit Worker() throw() :  total(0)
        {
        }

        virtual ~Worker() throw()
        {
        }

        void Exec( crew::context &ctx )
        {
            {
                YOCTO_LOCK(ctx.access);
                total = 0;
            }

            size_t ini = 0;
            size_t len = Count;
            ctx.split(ini,len);

            if(false)
            {
                ctx.access.lock();
                std::cerr << "==> Work " << ctx.size << "." << ctx.rank << ": " << ini << "+" << len << std::endl;
                ctx.access.unlock();
            }

            rand32_kiss r;
            r.seed(ctx.rank);
            double sum = 0;
            for(size_t j=0;j<len;++j)
            {
                for(size_t i=0;i<Loops;++i)
                {
                    sum += r.get<double>();
                }
            }

            tab[ctx.rank] = sum;
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Worker);
    };
    size_t Worker::Count = 10000;
    size_t Worker::Loops = 100;

    double Worker::tab[nTab] = {0};

}

#include "yocto/string/conv.hpp"

YOCTO_UNIT_TEST_IMPL(crew)
{

    crew::single_context mono;
    crew                 team(true);
    Worker               W;

    if( argc > 1 )
    {
        Worker::Count = strconv::to<size_t>(argv[1],"Count");
    }

    if( argc > 2 )
    {
        Worker::Loops = strconv::to<size_t>(argv[2],"Loops");
    }



    timings tmx;

    YOCTO_TIMINGS(tmx,2,W.Exec(mono));
    const double speed_seq = tmx.speed;
    {
        YOCTO_LOCK(team.access);
        std::cerr << "\t (*) speed_seq=" << speed_seq << std::endl;
    }

    YOCTO_TIMINGS(tmx,2,team(&W,&Worker::Exec));
    const double speed_par = tmx.speed;
    const double ratio     = speed_par/speed_seq;
    {
        YOCTO_LOCK(team.access);
        std::cerr << std::endl;
        std::cerr << "\t (*) speed_seq  = " << speed_seq << std::endl;
        std::cerr << "\t (*) speed_par  = " << speed_par << std::endl;
        std::cerr << "\t (*)     ratio  = " << ratio << std::endl;
        std::cerr << "\t (*) efficiency = " << (100.0*ratio)/team.size << " %" << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
