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


        void histogram::create(patches &hp, const graphix::patch &surface, threading::engine *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(hp,cpus,surface,true);
        }

        void histogram::finish(const patches &patches, threading::engine *server)
        {
            if(server) server->flush();
            for(size_t i=patches.size();i>0;--i)
            {
                const patch &sub = patches[i];
                for(size_t j=0;j<bins;++j)
                {
                    count[j] += sub.count[j];
                }
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

