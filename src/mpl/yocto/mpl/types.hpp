#ifndef YOCTO_MPL_TYPES_INCLUDED
#define YOCTO_MPL_TYPES_INCLUDED 1

#include "yocto/object.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"
#include <cstdlib>
#include <cerrno>

namespace yocto
{
    namespace mpl
    {
        typedef uint64_t word_t;

        class manager : public singleton<manager>
        {
        public:
            static const size_t                min_bytes = sizeof(word_t);
            static const size_t                max_bytes = 4096;
            static const double                sin_table[32];
            static const threading::longevity  life_time = object::provider::life_time - 2;
            static const char                  name[];

            static inline size_t align(size_t num_bytes)
            {
                return (num_bytes<=min_bytes) ? min_bytes : next_power_of_two(num_bytes);
            }

            //! byte field acquisition
            inline uint8_t *acquire(size_t &num_bytes)
            {
                num_bytes = align(num_bytes);
                if(num_bytes>max_bytes)
                {
                    YOCTO_GIANT_LOCK();
                    void *addr = calloc(1,num_bytes);
                    if(!addr) throw libc::exception(ENOMEM,"%s.acquire(%u)", name, unsigned(num_bytes) );
                    return (uint8_t *)addr;
                }
                else
                {
                    YOCTO_LOCK(access);
                    return (uint8_t *)(blocks.acquire(num_bytes));
                }
            }

            //! byte field release
            inline void release(uint8_t * &p, size_t &n)
            {
                assert(p);
                assert(n>=min_bytes);
                assert(is_a_power_of_two(n));
                if(n>max_bytes)
                {
                    YOCTO_GIANT_LOCK();
                    free(p);
                }
                else
                {
                    YOCTO_LOCK(access);
                    blocks.release(p,n);
                }
                p = 0;
                n = 0;
            }



        private:
            friend class singleton<manager>;
            explicit manager() throw();
            virtual ~manager() throw();

            memory::kBlocks blocks;
            
        };

        //! quick 0..size-1 array, powers of two
        template <typename T>
        class array_of
        {
        public:
            inline array_of(size_t n) :
            size(n),
            bytes(size*sizeof(T)),
            addr(0)
            {
                static manager &mgr = manager::instance();
                addr = (T*)( mgr.acquire(bytes) );
                size = bytes/sizeof(T);
            }

            inline ~array_of() throw()
            {
                static manager &mgr = *manager::location();
                uint8_t *p = (uint8_t *)addr;
                mgr.release(p,bytes);
            }

            const size_t size;

            inline T       & operator[](const size_t indx) throw()       { assert(indx<size); return  addr[indx]; }
            inline const T & operator[](const size_t indx) const throw() { assert(indx<size); return  addr[indx]; }

        private:
            size_t       bytes;
            T           *addr;

            YOCTO_DISABLE_COPY_AND_ASSIGN(array_of);
        };
    }
}

#endif
