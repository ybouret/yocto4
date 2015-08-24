#ifndef YOCTO_PARALLEL_SETUP_INCLUDED
#define YOCTO_PARALLEL_SETUP_INCLUDED 1

#include "yocto/parallel/split.hpp"
#include "yocto/container/slots.hpp"

namespace yocto
{
    namespace parallel
    {

        //! automatic setup
        /**
         USER_PATCH must have a 1-parameter constructor
         with a PATCH_TYPE.
         */
        template <
        typename USER_PATCH,
        typename PATCH_TYPE
        > inline
        void setup_patches(slots_of<USER_PATCH> &patches,
                           const size_t          cpus,
                           const PATCH_TYPE     &zone)
        {
            assert(0==patches.size);
            typename split_for<PATCH_TYPE::DIMENSIONS>::type splitter(cpus,zone);
            patches.resize(splitter.cores);
            for(register size_t rank=0;rank<splitter.cores;++rank)
            {
                const PATCH_TYPE sub = splitter(rank);
                patches.template append<PATCH_TYPE>(sub);
            }
        }

    }

}

#endif