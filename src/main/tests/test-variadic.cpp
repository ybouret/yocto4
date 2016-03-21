#include "yocto/utest/run.hpp"
#include <cstdarg>
#include <cstdio>
#include "yocto/code/rand.hpp"

using namespace yocto;

static inline int add_em_up (int count,...)
{
    va_list ap;
    int i, sum;

    va_start (ap, count);         /* Initialize the argument list. */

    sum = 0;
    for (i = 0; i < count; i++)
        sum += va_arg (ap, int);    /* Get the next argument value. */

    va_end (ap);                  /* Clean up. */
    return sum;
}

#include "yocto/memory/global.hpp"

template <typename T>
inline void varalloc()
{
    T *p,*q;
    const size_t n=100+alea_leq(100);
    size_t m = n;
    memory::kind<memory::global>::acquire_chunks<T>(m,2,&p,&q);
    std::cerr << "p@" << (void*)p << std::endl;
    std::cerr << "q@" << (void*)q << "/" << (void *)(p+n) << std::endl;
    memory::kind<memory::global>::release_as<T>(p,m); q=0;
}

YOCTO_UNIT_TEST_IMPL(variadic)
{
    /* This call prints 16. */
    printf ("%2d/16\n", add_em_up (3, 5, 5, 6));

    /* This call prints 55. */
    printf ("%2d/55\n", add_em_up (10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

    varalloc<int>();
    varalloc<double>();

}
YOCTO_UNIT_TEST_DONE()
