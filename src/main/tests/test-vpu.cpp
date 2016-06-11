#include "yocto/utest/run.hpp"
#include "yocto/threading/vpu.hpp"

using namespace yocto;


namespace
{
    class Processor
    {
    public:
        const int id;

        ~Processor() throw() {}
        Processor() throw() : id(0)
        {
            std::cerr << "default id=" << id << std::endl;
        }



    private:
        YOCTO_DISABLE_ASSIGN(Processor);

    };
}

YOCTO_UNIT_TEST_IMPL(vpu)
{
    threading::vpu::kexec_ptr             para( new threading::crew(true) );
    threading::vpu::kexec_ptr             mono( new threading::sequential_executor() );
    threading::processing_unit<Processor> cpu(para);
    
}
YOCTO_UNIT_TEST_DONE()
