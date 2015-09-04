#ifndef YOCTO_BIN2NAME_INCLUDED
#define YOCTO_BIN2NAME_INCLUDED 1

#include "yocto/code/round.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>

namespace yocto
{

    struct bin2name
    {
        static const char *head[16]; //!< consonant, 1 byte
        static const char *tail[16]; //!< vowel, 2 bytes

        static const char *get_head(const unsigned B) throw();
        static const char *get_tail(const unsigned B) throw();

        typedef uint16_t key_t;
        static  key_t    key_of(const void *data, const size_t size) throw();

        static const size_t key_size = sizeof(key_t);
        static const size_t out_size = 3 * key_size+1;
        static const size_t mem_size = YOCTO_ALIGN_FOR_ITEM(key_t,out_size);

        static void  fill(char *value, const key_t k) throw();
        static void  fill(char *value, const void *data, const size_t size) throw();
    };

    template <typename T>
    class bin2id
    {
    public:
        static const size_t out_size = 2*sizeof(T) + 1;
        static const size_t mem_size = YOCTO_ROUND4(out_size);

        const char value[mem_size];

        inline bin2id(const T *addr) throw() :
        value()
        {
            assert(addr);
            fill_with(addr);
        }

        inline bin2id(const bin2id &other) throw() :
        value()
        {
            memcpy((void*)value, other.value, sizeof(value));
        }

        inline bin2id & operator=(const bin2id &other) throw()
        {
            memmove((void*)value, other.value, sizeof(value));
            return *this;
        }

        inline ~bin2id() throw() {}

        inline bin2id & operator=(const T *addr) throw()
        {
            fill_with(addr);
            return *this;
        }

    private:
        inline void fill_with(const T *addr) throw()
        {
            const uint8_t *p = (const uint8_t *)addr;
            char          *q = (char          *)value;

            for(size_t i=0;i<sizeof(T);++i)
            {
                const char    *h = hexa_text[p[i]];
                const size_t   j = (i<<1);
                q[j+0] = h[0];
                q[j+1] = h[1];
            }
            
            for(size_t i=2*sizeof(T);i<mem_size;++i) q[i] = 0;
        }
    };
}

#endif
