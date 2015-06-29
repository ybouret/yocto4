#ifndef YOCTO_MK_EXTENDING_INCLUDED
#define YOCTO_MK_EXTENDING_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
    namespace math
    {

        enum extension_mode
        {
            extend_constant,
            extend_cyclic
        };

        template <typename T>
        class extension
        {
        public:
            explicit extension(const extension_mode lo, const extension_mode up) throw();
            virtual ~extension() throw();

            const extension_mode lower;
            const extension_mode upper;

            T get_x(const ptrdiff_t i, const array<T> &X, const ptrdiff_t N) const;



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(extension);
        };

    }
}

#endif
