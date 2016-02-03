#include "yocto/mpk/types.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>


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


    }
}

