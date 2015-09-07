#include "yocto/threading/crew.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand32.hpp"
#include "yocto/parallel/split.hpp"

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
            parallel::split::compute1D(ctx.rank, ctx.size, ini, len);

            if(false)
            {
                ctx.access.lock();
                std::cerr << "==> Work " << ctx.size << "." << ctx.rank << ": " << ini << "+" << len << std::endl;
                ctx.access.unlock();
            }

            rand32_kiss r;
            r.seed(ctx.rank);
            double sum = 0;
            for(size_t i=0;i<Loops;++i)
            {
                sum += r.get<double>();
            }

            {
                YOCTO_LOCK(ctx.access);
                total += sum;
            }
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Worker);
    };
    size_t Worker::Count = 1;
    size_t Worker::Loops = 1;
}

YOCTO_UNIT_TEST_IMPL(crew)
{

    crew::single_context mono;
    crew                 team;
    Worker               W;

    W.Count = 1000;
    W.Count = 0;

    W.Exec(mono);

    for(int cycle=1;cycle<=1000;++cycle)
    {
        team(&W,&Worker::Exec);
    }
    
}
YOCTO_UNIT_TEST_DONE()
