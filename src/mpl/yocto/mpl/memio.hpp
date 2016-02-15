#ifndef YOCTO_MPL_MEMIO_INCLUDED
#define YOCTO_MPL_MEMIO_INCLUDED 1

#include "yocto/object.hpp"
#include "yocto/threading/singleton.hpp"

namespace yocto
{
    namespace mpl
    {
        typedef uint64_t word_t;

        class memIO : public singleton<memIO>
        {
        public:
            static  const threading::longevity  life_time = object::provider::life_time - 2;
            static  const char                  name[];

            static  const double                sin_table[32];

        private:
            friend class singleton<memIO>;
            explicit memIO() throw();
            virtual ~memIO() throw();

            memory::kBlocks blocks;
            
        };
    }
}

#endif
