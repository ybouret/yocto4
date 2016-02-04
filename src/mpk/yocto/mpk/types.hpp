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
        typedef int64_t  wint_t;

        class manager : public singleton<manager>
        {
        public:
            static  const size_t                min_bytes = sizeof(word_t);
            static  const size_t                max_bytes = 4096;

            static  const threading::longevity  life_time = object::provider::life_time - 2;
            static  const char                  name[];

            static  const double                sin_table[32];

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

            template <typename T>
            inline void release_as(T * &p, size_t &n) throw()
            {
                assert(is_a_power_of_two(sizeof(T)));
                assert(p!=0);
                assert(is_a_power_of_two(n));
                size_t m = n * sizeof(T);
                release(p,m);
                p = 0;
                n = 0;
            }

        private:
            friend class singleton<manager>;
            explicit manager() throw();
            virtual ~manager() throw();

            memory::kBlocks blocks;
        };


        //! quick 0..size-1 array
        template <typename T>
        class __array_of
        {
        public:
            inline __array_of(size_t n) :
            size(n),
            addr(create(size))
            {
            }

            inline ~__array_of() throw()
            {
                static manager &mgr = * manager::location();
                mgr.release_as<T>(addr,size);
            }

            inline T & operator[](const size_t i) throw()
            {
                assert(i<size);
                return addr[i];
            }

            inline const T & operator[](const size_t i) const throw()
            {
                assert(i<size);
                return addr[i];
            }

        private:
            static inline T *create(size_t &n)
            {
                static manager &mgr = manager::instance();
                return mgr.acquire_as<T>(n);
            }
            size_t size;
            T     *addr;
            YOCTO_DISABLE_COPY_AND_ASSIGN(__array_of);
        };

    }
}

#endif
