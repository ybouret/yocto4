#ifndef YOCTO_PEARSON_HASHING_INCLUDED
#define YOCTO_PEARSON_HASHING_INCLUDED 1


#include "yocto/memory/buffer.hpp"
#include "yocto/core/list.hpp"
#include <iostream>

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
            uint8_t index_of( const uint8_t h ) const;

            void reset() throw();

            class Code
            {
            public:
                Code *next,*prev;
                const uint8_t data;

                Code( uint8_t C ) throw() : next(0), prev(0), data(C) {}
                ~Code() throw() {}

                YOCTO_MAKE_OBJECT

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Code);
            };

            class Word : public object, public core::list_of_cpp<Code>
            {
            public:
                Word   *next;
                Word   *prev;
                uint8_t H;

                Word( const void *buffer, const size_t buflen ) : next(0), prev(0), H(0)
                {
                    store(buffer,buflen);
                }

                Word( const memory::ro_buffer &buffer ) : next(0), prev(0), H(0)
                {
                    store(buffer.ro(), buffer.length());
                }

                virtual ~Word() throw() {}
                friend std::ostream & operator<<( std::ostream &os, const Word &w )
                {
                    for(const Code *c = w.head;c;c=c->next)
                    {
                        os << char(c->data);
                    }
                    return os;
                }
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Word);
                void store( const void *buffer, const size_t buflen )
                {
                    const uint8_t *C = (const uint8_t *)buffer;
                    for(size_t i=0;i<buflen;++i) push_back( new Code(C[i]) );
                }
            };

            typedef core::list_of_cpp<Word> Words;

            void build( Words &words );

        };
        
    }
}

#endif

