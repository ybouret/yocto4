#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto {

    namespace math
    {

        template <typename T, typename U>
        class Bsplines
        {
        public:

            //!
            /**
             \param t assume increasingly ordered
             */
            T compute( const T x, const array<T> &t, const array<U> &P, const size_t n )
            {
                assert(t.size()>=2);
                assert(t.size()==P.size());
                assert(n<=t.size());
                
                return 0;
            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Bsplines);
        };

    }
}

#endif
