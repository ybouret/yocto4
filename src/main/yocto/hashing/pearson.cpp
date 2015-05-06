#include "yocto/hashing/pearson.hpp"
#include <cstring>

namespace yocto
{
    namespace hashing
    {

        void Pearson:: reset() throw()
        {
            for(unsigned i=0;i<256;++i)
            {
                table[i] = uint8_t(i);
            }
        }


        Pearson:: Pearson() throw() :
        table()
        {
            reset();
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

#include "yocto/exception.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/sort/quick.hpp"

#include <iostream>

namespace yocto
{
    namespace hashing
    {
        uint8_t Pearson:: index_of( const uint8_t h ) const
        {
            for(unsigned i=0;i<256;++i)
            {
                if(h==table[i]) return i;
            }
            throw exception("invalid Pearson Table, missing %u!", unsigned(h) );
        }

        void Pearson:: fill_I(int *I, int &top, const void *data, const size_t size, uint8_t h)
        {
            assert(!(data==NULL&&size>0));
            const uint8_t *C = (const uint8_t *)data;
            --C;

            for(size_t q=size;q>0;--q)
            {
                if(I[h]<=0)
                {
                    if(top>255) throw exception("-- fill FAILURE --");
                    I[h] = top++;
                    std::cerr << "I[" << int(h) << "]=" << I[h] << std::endl;
                }
                h = I[h] ^ C[q];
            }
        }


        void Pearson::  finish(int *I, int &top)
        {
            for(int i=0;i<256;++i)
            {
                if(I[i]<0)
                {
                    if(top>255) throw exception("-- finish FAILURE --");
                    I[i] = top++;
                }
            }
        }

        void Pearson:: build_from(int *I)
        {
            reset();
            lw_array<int>     a(I,256);
            lw_array<uint8_t> b(table,256);
            co_qsort(a,b);

            std::cerr << "table=" << std::endl;
            std::cerr.flush();
            for(size_t i=0,k=0;i<16;++i)
            {
                for(size_t j=0;j<16;++j)
                {
                    fprintf(stderr,"%3d ", int(table[k++]));
                }
                fprintf(stderr, "\n");
            }
            fflush(stderr);
        }

    }
}
