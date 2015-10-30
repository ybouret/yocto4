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

#define Y_GFX_HIST_RESET(I) count[I] = 0

        void histogram:: reset() throw()
        {
            YOCTO_LOOP_FUNC_(bins,Y_GFX_HIST_RESET,0);
        }


#define Y_GFX_HIST_MERGE(I) count[I] += h.count[I]

        void histogram:: merge(const histogram &h) throw()
        {
            YOCTO_LOOP_FUNC_(bins,Y_GFX_HIST_MERGE,0);
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
        histogram_patch:: ~histogram_patch() throw()
        {
        }

        histogram_patch:: histogram_patch(const patch &p) throw() :
        patch(p),
        src(0)
        {
        }
        
    }

}

