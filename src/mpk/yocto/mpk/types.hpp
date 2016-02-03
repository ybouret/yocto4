#ifndef YOCTO_MPK_TYPES_INCLUDED
#define YOCTO_MPK_TYPES_INCLUDED 1

#include "yocto/object.hpp"
#include "yocto/memory/kblocks.hpp"

namespace yocto
{
    namespace mpk
    {


        class allocator : public singleton<allocator>
        {
        public:
            typedef uint64_t                   word_t;
            static const size_t                min_bytes = sizeof(word_t);
            static  const size_t               max_bytes = 4096;
            static  const threading::longevity life_time = object::provider::life_time - 2;
            static  const char                 name[];

            static size_t align(size_t n);

        private:
            friend class singleton<allocator>;
            explicit allocator() throw();
            virtual ~allocator() throw();

            memory::kBlocks blocks;
        };
    }
}

#endif
