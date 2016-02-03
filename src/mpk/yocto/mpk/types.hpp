#ifndef YOCTO_MPK_TYPES_INCLUDED
#define YOCTO_MPK_TYPES_INCLUDED 1

#include "yocto/object.hpp"
#include "yocto/memory/kblocks.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace mpk
    {

        typedef uint64_t word_t;

        class manager : public singleton<manager>
        {
        public:
            static  const size_t                min_bytes = sizeof(word_t);
            static  const size_t                max_bytes = 4096;
            static  const threading::longevity  life_time = object::provider::life_time - 2;
            static  const char                  name[];

            static size_t align(size_t n);

            //! acquire a block of aligned memory
            void  *acquire(size_t &n);

            //! release a previously allocated block
            void   release(void *p, size_t n) throw();

            template <typename T>
            inline T *acquire_as(size_t &n)
            {
                assert(is_a_power_of_two(sizeof(T)));
                size_t m = n * sizeof(T);
                try
                {
                    void *p = acquire(m);
                    n=m/sizeof(T);
                    return static_cast<T*>(p);
                }
                catch(...)
                {
                    n = 0;
                    throw;
                }
            }

        private:
            friend class singleton<manager>;
            explicit manager() throw();
            virtual ~manager() throw();

            memory::kBlocks blocks;
        };
    }
}

#endif
