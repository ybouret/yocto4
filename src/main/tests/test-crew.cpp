#include "yocto/threading/crew.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand32.hpp"

using namespace yocto;
using namespace threading;

namespace
{

    class Worker
    {

    public:

        explicit Worker()
        {
        }

        virtual ~Worker() throw()
        {
        }

        void Exec( crew::context &ctx )
        {
            ctx.access.lock();
            //std::cerr << "==> Work " << ctx.size << "." << ctx.rank << std::endl;
            ctx.access.unlock();

            rand32_kiss r;
            r.seed(ctx.rank);
            volatile double sum = 0;
            for(size_t i=0;i<100;++i)
            {
                sum += r.get<double>();
            }
            
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Worker);
    };
}

YOCTO_UNIT_TEST_IMPL(crew)
{

    crew::single_context mono;
    crew                 team;
    Worker               W;

    W.Exec(mono);

    for(int cycle=1;cycle<=10;++cycle)
    {
        team(&W,&Worker::Exec);
    }
    
}
YOCTO_UNIT_TEST_DONE()
