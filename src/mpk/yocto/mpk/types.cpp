#include "yocto/mpk/types.hpp"
#include "yocto/exceptions.hpp"
#include <cstdlib>
#include <cerrno>
#include <cmath>

namespace yocto
{
    namespace mpk
    {

        const char manager::name[] = "mpk";
        
        manager:: ~manager() throw()
        {
        }

        manager::manager() throw() :
        singleton<manager>(),
        blocks(max_bytes)
        {

        }
        
        size_t manager::align(size_t n)
        {
            if(n<=min_bytes)
            {
                assert(is_a_power_of_two(min_bytes));
                return min_bytes;
            }
            else
            {
                const size_t r = next_power_of_two(n);
                if(r<n) throw libc::exception(EINVAL,"mpk size overflow!");
                assert(is_a_power_of_two(r));
                return r;
            }
        }


        void  * manager::acquire(size_t &n)
        {
            n = align(n);
            if(n>max_bytes)
            {
                YOCTO_GIANT_LOCK();
                void *p = calloc(1,n);
                if(!p)
                {
                    throw libc::exception(ENOMEM,"mpk memory acquire");
                    n = 0;
                }
                return p;
            }
            else
            {
                YOCTO_LOCK(access);
                try
                {
                    return blocks.acquire(n);
                }
                catch(...)
                {
                    n=0;
                    throw;
                }
            }
        }


        void manager:: release(void *p, size_t n) throw()
        {
            assert(p);
            assert(n>0);
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

        }

#define SIN_OF(LN2) sin(6.28318530717958623199592693708837032318115234375/(size_t(1)<<(LN2)))
        const double manager::sin_table[32] =
        {
            SIN_OF(0),  SIN_OF(1),  SIN_OF(2),  SIN_OF(3),  SIN_OF(4),
            SIN_OF(5),  SIN_OF(6),  SIN_OF(7),  SIN_OF(8),  SIN_OF(9),
            SIN_OF(10), SIN_OF(11), SIN_OF(12), SIN_OF(13), SIN_OF(14),
            SIN_OF(15), SIN_OF(16), SIN_OF(17), SIN_OF(18), SIN_OF(19),
            SIN_OF(20), SIN_OF(21), SIN_OF(22), SIN_OF(23), SIN_OF(24),
            SIN_OF(25), SIN_OF(26), SIN_OF(27), SIN_OF(28), SIN_OF(29),
            SIN_OF(30), SIN_OF(31)
        };

    }
}

