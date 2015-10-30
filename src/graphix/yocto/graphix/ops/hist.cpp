#include "yocto/graphix/ops/hist.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace graphix
    {

        histogram:: ~histogram() throw()
        {
        }

        histogram:: histogram() throw() : count()
        {
            reset();
        }

#define Y_GFX_HIST_RESET(I) arr[I] = 0
        void histogram:: __reset(word_type *arr) throw()
        {
            assert(arr);
            YOCTO_LOOP_FUNC_(bins,Y_GFX_HIST_RESET,0);
        }

        void histogram:: reset() throw()
        {
            __reset(count);
        }


        void histogram:: save(const string &hname) const
        {
            ios::wcstream fp(hname);
            for(size_t i=0;i<bins;++i)
            {
                fp("%u %g\n", unsigned(i), double(count[i]));
            }
        }


    }

}



namespace yocto
{
    namespace graphix
    {
        histogram::patch:: ~patch() throw()
        {
        }

        histogram::patch:: patch(const graphix::patch &p):
        graphix::patch(p),
        src(0),
        count()
        {
        }        
        
    }
    
}

