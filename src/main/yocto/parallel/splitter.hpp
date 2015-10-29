#ifndef YOCTO_PARALLEL_SPLITTER_INCLUDED
#define YOCTO_PARALLEL_SPLITTER_INCLUDED 1

#include "yocto/parallel/split1d.hpp"
#include "yocto/parallel/split2d.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace parallel
    {

        template <size_t N>
        struct splitter_for;

        template <> struct splitter_for<1> { typedef split1D type; };
        template <> struct splitter_for<2> { typedef split2D type; };

        template <
        typename USER_PATCH,
        typename PATCH_TYPE>
        void build_patches(vector<USER_PATCH> &patches,
                           const size_t        cpus,
                           const PATCH_TYPE   &zone)
        {
            patches.free();
            typedef  typename splitter_for<PATCH_TYPE::DIM>::type __split_type;
            typedef  typename __split_type::patch_type            __patch_type;
            const __split_type S(cpus,zone);
            patches.ensure(S.cores);
            for(size_t rank=0;rank<S.cores;++rank)
            {
                const __patch_type sub = S(rank);
                patches.template append<const __patch_type&>(sub);
            }
        }
        
    }
}

#endif

