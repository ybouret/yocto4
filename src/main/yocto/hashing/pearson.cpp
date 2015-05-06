#include "yocto/hashing/pearson.hpp"
#include <cstring>

namespace yocto
{
    namespace hashing
    {

        Pearson:: Pearson() throw() :
        table()
        {
            for(unsigned i=0;i<256;++i)
            {
                table[i] = uint8_t(i);
            }
        }

        Pearson:: ~Pearson() throw()
        {
        }

        Pearson:: Pearson( const Pearson &other) throw() :
        table()
        {
            memcpy(table, other.table, sizeof(table) );
        }

        Pearson & Pearson:: operator= ( const Pearson &other) throw()
        {
            memmove(table, other.table, sizeof(table) );
            return *this;
        }

        uint8_t Pearson:: operator()( const void *data, const size_t size ) const throw()
        {
            assert(!(data==NULL&&size>0));
            uint8_t        h = 0;
            const uint8_t *C = (const uint8_t *)data;
            for(size_t i=0;i<size;++i)
            {
                h = table[ C[i] ^ h ];
            }
            return h;
        }

        uint8_t Pearson:: operator()( const memory::ro_buffer &buff ) const throw()
        {
            const Pearson &self = *this;
            return self( buff.ro(), buff.length() );
        }



    }

}
