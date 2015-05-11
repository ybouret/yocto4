#ifndef YOCTO_HASHING_PERFECT_INCLUDED
#define YOCTO_HASHING_PERFECT_INCLUDED 1

#include "yocto/core/pool.hpp"
#include "yocto/string.hpp"

#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace hashing
    {

        class perfect
        {
        public:

            class node_type
            {
            public:
                typedef core::pool_of_cpp<node_type> list_type;
                node_type     *next;
                const int      hash;
                list_type      chld;
                const uint8_t  code;

                node_type(const uint8_t C) throw();
                ~node_type() throw();

                void viz( ios::ostream &fp ) const;

                YOCTO_MAKE_OBJECT

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
            };

            explicit perfect();
            explicit perfect( const char **words, const size_t count);
            virtual ~perfect() throw();

            void insert( const void *data, const size_t size, const int h);
            void insert( const memory::ro_buffer &buff,       const int h);
            void insert( const char *txt,                     const int h);


            int  operator()(const void *data, const size_t size) const throw();
            int  operator()(const memory::ro_buffer &buff) const throw();
            int  operator()(const char *txt) const throw();

            void optimize() throw();

            void graphviz( const string &filename ) const;

        private:
            node_type *root;
            static void reverse(node_type*) throw();

            YOCTO_DISABLE_COPY_AND_ASSIGN(perfect);
        public:
            const size_t nodes;
        };

#define YOCTO_PERFECT_HASHER_FOR(WORDS) WORDS,sizeof(WORDS)/sizeof(WORDS[0])
    }
}

#endif
