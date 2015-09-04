#ifndef YOCTO_BIN2NAME_INCLUDED
#define YOCTO_BIN2NAME_INCLUDED 1

#include "yocto/code/round.hpp"
#include <cstring>

namespace yocto
{

    struct bin2name
    {
        static const char *head[16]; //!< consonant, 1 byte
        static const char *tail[16]; //!< vowel, 2 bytes

        static const char *get_head(const unsigned B) throw();
        static const char *get_tail(const unsigned B) throw();

        typedef uint32_t key_t;
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
        const char value[bin2name::mem_size];

        inline bin2id(const T *addr) throw() :
        value()
        {
            assert(addr);
            bin2name::fill((char*)value,addr,sizeof(T));
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
            bin2name::fill((char*)value,addr,sizeof(T));
            return *this;
        }

    };
}

#endif
