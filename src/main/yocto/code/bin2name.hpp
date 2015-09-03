#ifndef YOCTO_BIN2NAME_INCLUDED
#define YOCTO_BIN2NAME_INCLUDED 1

#include "yocto/code/unroll.hpp"
#include <cstring>

namespace yocto
{

    struct bin2name
    {
        static const char *head[16]; //!< consonant, 1 byte
        static const char *tail[16]; //!< vowel, 2 bytes

        static const char *get_head(const unsigned B) throw();
        static const char *get_tail(const unsigned B) throw();
    };

    template <typename T>
    class bin2id
    {
    public:
        static const size_t bytes = 3*sizeof(T);
        const char value[bytes+1];

        inline bin2id(const T *addr) throw() :
        value()
        {
            assert(addr);
            memset((void*)value,0,sizeof(value));
            fill_value_with(addr);
        }

        inline bin2id(const bin2id &other) throw() :
        value()
        {
            memcpy((void*)value, other.value, sizeof(value));
        }


        inline ~bin2id() throw() {}


    private:
        YOCTO_DISABLE_ASSIGN(bin2id);

#define YOCTO_BIN2ID(INDEX)    \
const unsigned B = p[INDEX];   \
const size_t   j = 3*(INDEX);  \
const char    *h = bin2name::get_head(B);\
s[j+0] = h[0];\
const char    *t = bin2name::get_tail(B);\
s[j+1] = t[0]; \
s[j+2] = t[1]

        inline void fill_value_with(const T *addr) throw()
        {
            const uint8_t *p   = (const uint8_t *)addr;
            char          *s   = (char *)value;
            YOCTO_LOOP_FUNC_(sizeof(T), YOCTO_BIN2ID, 0);
        }

#undef YOCTO_BIN2ID
    };
}

#endif
