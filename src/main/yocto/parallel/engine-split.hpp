#ifndef  YOCTO_PARALLEL_ENGINE_SPLIT_INCLUDED
#define  YOCTO_PARALLEL_ENGINE_SPLIT_INCLUDED 1

#include "yocto/parallel/splitter.hpp"
#include "yocto/threading/engine.hpp"

namespace yocto
{
    namespace parallel
    {

        template <
        typename USER_PATCH,
        typename PATCH_TYPE
        >
        void create_patches(vector<USER_PATCH> &patches,
                            const PATCH_TYPE   &zone,
                            threading::engine  *server)
        {
            const size_t cpus = (0!=server) ? server->size : 1;
            build_patches<USER_PATCH,PATCH_TYPE>(patches,cpus,zone);
        }

    }
}

#endif
