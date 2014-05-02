#ifndef YOCTO_THREADING_WINDOW_INCLUDED
#define YOCTO_THREADING_WINDOW_INCLUDED 1

#include "yocto/threading/context.hpp"

namespace yocto
{
    namespace threading
    {
        //! can be used as a base class for data segmenting
        class window
        {
        public:
            explicit window(const context &, size_t length, size_t offset ) throw();
            virtual ~window() throw();
            window(const window &w) throw();
            const size_t start;
            const size_t count;
            const size_t final;
        private:
            YOCTO_DISABLE_ASSIGN(window);
        };

    }
}

#endif
