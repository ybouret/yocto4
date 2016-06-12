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

        inline void run( threading::context &ctx, array<double> &data ) throw()
        {
            size_t offset = 1;
            size_t length = data.size();
            ctx.split(offset, length);
            localSum = 0;
#define DO_SUM(I) localSum += data[I]
            YOCTO_LOOP_FUNC(length, DO_SUM, offset);
            {
                YOCTO_LOCK(ctx.access);
                { std::cerr << "localSum" << ctx.rank << "=" << localSum << std::endl; }
            }
        }
                
        inline void run( threading::context &ctx, array<double> &odata, const array<int> &idata ) throw()
        {
            size_t offset = 1;
            size_t length = idata.size();
            ctx.split(offset, length);
            localSum = 0;
            for(size_t i=offset,count=length;count-->0;++i)
            {
                odata[i] = idata[i];
                localSum += odata[i];
            }
            {
                YOCTO_LOCK(ctx.access);
                { std::cerr << "localSum" << ctx.rank << "=" << localSum << std::endl; }
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

    vector<double> data(1000);
    vector<int>    idata(1000);

    for(size_t i=1;i<=data.size();++i)
    {
        data[i]  = 1;
        idata[i] = i;
    }
    std::cerr << "one array" << std::endl;
    cpu.compile<double>();
    cpu.call(data);
    
    std::cerr << std::endl << "two arrays" << std::endl;
    cpu.compile<double,int>();
    cpu.call(data,idata);
}
YOCTO_UNIT_TEST_DONE()
