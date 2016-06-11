#include "yocto/utest/run.hpp"
#include "yocto/threading/vpu.hpp"
#include "yocto/code/unroll.hpp"
using namespace yocto;


namespace
{
    class Processor
    {
    public:
        const int id;
        double    localSum;

        ~Processor() throw() {}
        Processor() throw() : id(0), localSum(0)
        {
            std::cerr << "default id=" << id << std::endl;
        }

        inline void operator()( threading::context &ctx, array<double> &data ) throw()
        {

            size_t offset = 1;
            size_t length = data.size();
            ctx.split(offset, length);
            localSum = 0;
#define DO_SUM(I) localSum += data[I]
            YOCTO_LOOP_FUNC(length, DO_SUM, offset);
            {
                YOCTO_LOCK(ctx.access);
                std::cerr << "localSum=" << localSum << std::endl;
            }
        }


    private:
        YOCTO_DISABLE_ASSIGN(Processor);

    };
}

#include "yocto/sequence/vector.hpp"

YOCTO_UNIT_TEST_IMPL(vpu)
{
    threading::vpu::kexec_ptr             para( new threading::crew(true) );
    threading::vpu::kexec_ptr             mono( new threading::sequential_executor() );
    threading::processing_unit<Processor> cpu(para);

    for(size_t i=0;i<cpu.cores;++i)
    {
        cpu.push_back();
    }

    cpu.compile<double>();

    vector<double> data(1000);
    for(size_t i=1;i<=data.size();++i) data[i] = i;
    
    cpu.call(data);
}
YOCTO_UNIT_TEST_DONE()
