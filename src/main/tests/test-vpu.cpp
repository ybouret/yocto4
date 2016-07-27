#include "yocto/utest/run.hpp"
#include "yocto/threading/vpu.hpp"
#include "yocto/code/unroll.hpp"
#include <cmath>
using namespace yocto;


namespace
{
    class Processor
    {
    public:
        const int id;
        double    localSum;

        void do_work()
        {
            volatile double sum = 0;
            for(size_t i=5000;i>0;--i)
            {
                sum += cos( i*3.14 );
            }
        }

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
#define DO_SUM(I) localSum += data[I]; do_work()
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
                do_work();
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
                do_work();
            }

        }



    private:
        YOCTO_DISABLE_ASSIGN(Processor);

    };
}

#include "yocto/sequence/vector.hpp"
#include "yocto/sys/rt-clock.hpp"

#define INIT_DATA() do { \
for(size_t i=1;i<=data.size();++i)\
{\
    data[i]  = 1;\
    idata[i] = int(i);\
    sdata[i] = float(i);\
}\
mark = rt_clock::ticks();\
} while(false)

YOCTO_UNIT_TEST_IMPL(vpu)
{
    threading::processing_unit<Processor> cpu(new threading::crew(true));
    threading::processing_unit<Processor> mono(new threading::sequential_executor());

    for(size_t i=0;i<cpu.cores;++i)
    {
        cpu.push_back();
    }
    mono.push_back();

    const size_t   N = 10000;
    vector<double> data(N);
    vector<int>    idata(N);
    vector<float>  sdata(N);



    std::cerr << "one array" << std::endl;
    uint64_t mark = 0;

    cpu.compile<double>();
    mono.compile<double>();

    INIT_DATA();
    mono.call(data,NULL);
    const uint64_t seq1 = rt_clock::ticks() - mark;

    INIT_DATA();
    cpu.call(data,NULL);
    const uint64_t par1 = rt_clock::ticks() - mark;
    for(size_t i=0;i<cpu.cores;++i)
    {
        std::cerr << "localSum" << i << "=" << cpu[i].localSum << std::endl;
    }
    std::cerr << "speedup1=" << double(seq1)/double(par1) << std::endl;

    std::cerr << std::endl << "two arrays" << std::endl;
    mono.compile<double,int>();
    cpu.compile<double,int>();

    INIT_DATA();
    mono.call(data,idata,NULL);
    const uint64_t seq2 = rt_clock::ticks() - mark;

    INIT_DATA();
    cpu.call(data,idata,NULL);
    const uint64_t par2 = rt_clock::ticks() - mark;
    for(size_t i=0;i<cpu.cores;++i)
    {
        std::cerr << "localSum" << i << "=" << cpu[i].localSum << std::endl;
    }
    std::cerr << "speedup2=" << double(seq2)/double(par2) << std::endl;


    std::cerr << std::endl << "three arrays" << std::endl;
    cpu.compile<double,int,float>();
    mono.compile<double,int,float>();

    INIT_DATA();
    mono.call(data,idata,sdata,NULL);
    const uint64_t seq3 = rt_clock::ticks() - mark;

    INIT_DATA();
    cpu.call(data,idata,sdata,NULL);
    const uint64_t par3 = rt_clock::ticks() - mark;
    for(size_t i=0;i<cpu.cores;++i)
    {
        std::cerr << "localSum" << i << "=" << cpu[i].localSum << std::endl;
    }
    std::cerr << "speedup3=" << double(seq3)/double(par3) << std::endl;

}
YOCTO_UNIT_TEST_DONE()
