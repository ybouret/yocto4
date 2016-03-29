#ifndef YOCTO_HASHING_MPH_INCLUDED
#define YOCTO_HASHING_MPH_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/string.hpp"
#include "yocto/ios/ostream.hpp"


namespace yocto
{
    namespace hashing
    {
        class mperf
        {
        public:
            class node_type
            {
            public:
                typedef core::list_of_cpp<node_type> list_type;
                node_type      *next; //!< for list
                node_type      *prev; //!< for list
                const int       hash; //!< in-order hash code
                const uint8_t   code; //!< associated byte
                list_type       chld; //!< child(ren) nodes
                unsigned        freq; //!< frequency
                
                ~node_type() throw();
                node_type(const uint8_t C) throw();

                void viz( ios::ostream &fp ) const; //!< output graphviz
                void optimize() throw(); //!< order by decreasing frequency

                YOCTO_MAKE_OBJECT

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
            };

            explicit mperf();          //!< default constructor
            explicit mperf(const char **words,const size_t count);
            virtual ~mperf() throw();  //!< delete root

            //! insert a new data in tree, h>=0
            void insert(const void *data, const size_t size, const int h);

            //! wrapper for litterals
            inline void insert(const char *text, const int h)
            {
                insert(text,length_of(text),h);
            }

            //! wrapper for buffers
            inline void insert(const memory::ro_buffer &buf, const int h)
            {
                insert(buf.ro(),buf.length(),h);
            }

            //! propagate optimization
            inline void optimize() throw() { root->optimize(); }


            //! get the hash code
            int find(const void *data, const size_t size) const throw();

            //! get the hash code for litterals
            inline int operator()(const char *text) const throw()
            {
                return find(text,length_of(text));
            }

            //! get the hash code for buffers
            inline int operator()(const memory::ro_buffer &buf) const throw()
            {
                return find(buf.ro(),buf.length());
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(mperf);
            node_type *root;

        public:
            const size_t nodes;
            void graphviz( const string &filename ) const;
        };
    }
    
#define YOCTO_MPERF_FOR(WORDS) WORDS,sizeof(WORDS)/sizeof(WORDS[0])

}

#endif
