#ifndef YOCTO_PEARSON_HASHING_INCLUDED
#define YOCTO_PEARSON_HASHING_INCLUDED 1


#include "yocto/memory/buffer.hpp"

namespace yocto
{
    namespace hashing
    {

        class Pearson
        {
        public:
            static const size_t  NUM = 256;
            uint8_t table[NUM];

            explicit Pearson() throw();
            virtual ~Pearson() throw();
            Pearson(const Pearson &P) throw();
            Pearson & operator=(const Pearson &other) throw();

            uint8_t operator()( const void *data, const size_t size ) const throw();
            uint8_t operator()( const memory::ro_buffer &buff ) const throw();

        };

    }
}

#endif

