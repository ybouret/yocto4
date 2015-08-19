#ifndef YOCTO_CODE_PARALLEL_SPLIT_INCLUDED
#define YOCTO_CODE_PARALLEL_SPLIT_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{

    class parallel_patch1D
    {
    public:
        const unit_t start;
        const unit_t count;
        const unit_t final;

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(parallel_patch1D);
    };

}

#endif

