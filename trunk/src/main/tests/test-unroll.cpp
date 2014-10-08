#include "yocto/utest/run.hpp"
#include "yocto/code/unroll.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(unroll)
{
    static const size_t count_max = 1000;
    static const size_t shift_max = 1000;
    size_t i=0;
    
    std::cerr << "Testing LOOP" << std::endl;
    for(size_t count=0;count<=count_max;++count)
    {
        i=0;
        YOCTO_LOOP( count, i++ );
        if(i!=count)
        {
            throw exception("invalid loop %u", unsigned(count) );
        }
    }
    
    std::cerr << "Testing LOOP_" << std::endl;
    for(size_t count=1;count<=count_max;++count)
    {
        i=0;
        YOCTO_LOOP_( count, i++ );
        if(i!=count)
        {
            throw exception("invalid loop_ %u", unsigned(count) );
        }
    }
    
    size_t arr[2048];
#define Y_LOOP_CB(I) arr[I] = I
    
    std::cerr << "Testing FUNC" << std::endl;
    for(size_t count=0;count<=count_max;++count)
    {
        for(size_t shift=0;shift<=shift_max;++shift)
        {
            YOCTO_LOOP_FUNC(count, Y_LOOP_CB, shift);
            for(size_t i=shift;i<shift+count;++i)
            {
               if(arr[i]!=i)
                   throw exception("invalid FUNC count=%u, shift=%u", unsigned(count), unsigned(shift) );
            }
        }
    }
    
    std::cerr << "Testing FUNC_" << std::endl;
    for(size_t count=1;count<=count_max;++count)
    {
        for(size_t shift=0;shift<=shift_max;++shift)
        {
            YOCTO_LOOP_FUNC_(count, Y_LOOP_CB, shift);
            for(size_t i=shift;i<shift+count;++i)
            {
                if(arr[i]!=i)
                    throw exception("invalid FUNC count=%u, shift=%u", unsigned(count), unsigned(shift) );
            }
        }
    }
    

    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
