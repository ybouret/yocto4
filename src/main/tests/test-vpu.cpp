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

        inline void run(threading::context &ctx, array<double> &data , void *) throw()
        {
            size_t offset = 1;
            size_t length = data.size();
            ctx.split(offset, length);
            localSum = 0;
#define DO_SUM(I) localSum += data[I]
            YOCTO_LOOP_FUNC(length, DO_SUM, offset);

        }

        inline void run( threading::context &ctx, array<double> &odata, const array<int> &idata , void *) throw()
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

        }

        inline void run( threading::context &ctx, array<double> &odata, const array<int> &idata , const array<float> &sdata, void *) throw()
        {
            size_t offset = 1;
            size_t length = idata.size();
            ctx.split(offset, length);
            localSum = 0;
            for(size_t i=offset,count=length;count-->0;++i)
            {
                odata[i] = idata[i];
                localSum += odata[i];
                localSum += double(sdata[i]);
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
    vector<float>  sdata(1000);

    for(size_t i=1;i<=data.size();++i)
    {
        data[i]  = 1;
        idata[i] = i;
        sdata[i] = i+1;
    }
    std::cerr << "one array" << std::endl;
    cpu.compile<double>();
    cpu.call(data,NULL);
    for(size_t i=0;i<cpu.cores;++i)
    {
        std::cerr << "localSum" << i << "=" << cpu[i].localSum << std::endl;
    }

    std::cerr << std::endl << "two arrays" << std::endl;
    cpu.compile<double,int>();
    cpu.call(data,idata,NULL);
    for(size_t i=0;i<cpu.cores;++i)
    {
        std::cerr << "localSum" << i << "=" << cpu[i].localSum << std::endl;
    }

    std::cerr << std::endl << "three arrays" << std::endl;
    cpu.compile<double,int,float>();
    cpu.call(data,idata,sdata,NULL);
    for(size_t i=0;i<cpu.cores;++i)
    {
        std::cerr << "localSum" << i << "=" << cpu[i].localSum << std::endl;
    }

}
YOCTO_UNIT_TEST_DONE()
