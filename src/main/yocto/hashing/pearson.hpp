#ifndef YOCTO_PEARSON_HASHING_INCLUDED
#define YOCTO_PEARSON_HASHING_INCLUDED 1


#include "yocto/memory/buffer.hpp"
#include "yocto/core/list.hpp"

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

            class node_t
            {
            public:
                node_t        *prev;
                node_t        *next;
                const  uint8_t code;
                node_t(uint8_t b) throw() : prev(0), next(0), code(b) {}
                ~node_t() throw() {}

                YOCTO_MAKE_OBJECT

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node_t);
            };

            class list_t : public core::list_of_cpp<node_t>
            {
            public:
                list_t() throw() {}
                virtual ~list_t() throw() {}

                void initialize()
                {
                    clear();
                    for(unsigned i=0;i<256;++i)
                    {
                        push_back( new node_t(i) );
                    }
                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(list_t);
            };

            void fill_I( int *I, list_t &L, const void *data, const size_t size, const uint8_t H);

            void  build_from(int *I);

        };
        
    }
}

#endif

