#ifndef YOCTO_PACK_HUFFMAN_INCLUDED
#define YOCTO_PACK_HUFFMAN_INCLUDED 1

#include "yocto/pack/q-codec.hpp"
#include "yocto/ordered/heap.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/ios/bitio.hpp"

namespace yocto
{
    namespace pack
    {


        struct huffman
        {

            class code_bit
            {
            public:
                code_bit *next;
                code_bit *prev;
                bool       flag;
                YOCTO_MAKE_OBJECT
                inline  code_bit() throw() : next(0), prev(0), flag(false) {}
                inline ~code_bit() throw() {}

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(code_bit);
            };

            typedef core::list_of_cpp<code_bit> code_type;
            typedef core::pool_of_cpp<code_bit> code_pool;
            typedef int                         char_type; //!< internal representation of chars
            typedef size_t                      freq_type; //!< frequency



            static const size_t    max_bytes = 256; //!< regular bytes
            static const size_t    num_ctrls = 2;   //!< control code
            static const size_t    max_items = max_bytes+num_ctrls;
            static const char_type NYT       = max_bytes;
            static const char_type END       = NYT+1;
            static const char_type INS       = -1;

            //! one item of alphabet
            class item_type
            {
            public:
                const char_type data; //!< identity
                freq_type       freq; //!< frequency
                code_type       code; //!< current code
                
                item_type(const char_type data) throw();

                friend std::ostream & operator<<(std::ostream &, const item_type &);

                void emit( ios::bitio &IO ) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(item_type);
                ~item_type() throw();
            };

            static const char *item_text(const char_type ch) throw();
            static const char *code_text(const code_type &code);

            //! the alphabet, manage max_items and their frequencies
            class alphabet
            {
            public:
                alphabet( code_pool &cache );
                ~alphabet() throw();
                void initialize( code_pool &cache ) throw(); //!< assign code/bits/freq
                void discard_in( code_pool &cache ) throw(); //!< clean up codes

                void rescale() throw();    //!< scale down frequencies

                void update(const char c) throw();
                
                item_type & operator[](const size_t i) throw();

                void display( std::ostream &os ) const;

                const size_t     size;  //!< num items
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(alphabet);
                size_t     itnum; //!< for memory
                item_type *items; //!< for items
            };

            static const size_t max_nodes = (2 * max_items) - 1;
            static const size_t max_bits  = 16;

            //! node to build encoding tree
            class node_type
            {
            public:
                node_type      *left;   //!< left node
                node_type      *right;  //!< right node
                node_type      *parent; //!< parent node
                const freq_type freq;   //!< cumulative frequency
                const char_type data;   //!< associated data: INS,0..MAX_ITEMS-1
                size_t          bits;   //!< height
                bool            flag;   //!< coding bit

                node_type(const char_type ch, const freq_type fr) throw();

                void viz(ios::ostream &fp) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
                ~node_type() throw();
            };


            //! class for internal heap
            struct node_comparator
            {
                inline int operator()(const node_type &lhs, const node_type &rhs) const throw()
                {
                    return __compare_decreasing(lhs.freq,rhs.freq);
                }
            };

            //! internal heap
            typedef heap<node_type,node_comparator> heap_type;

            //! a huffman tree
            class tree_type
            {
            public:
                tree_type();
                ~tree_type() throw();

                node_type *build_for( alphabet &alpha );
                void graphviz(const string &filename) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree_type);
                size_t     count; //!< for memory
                node_type *nodes; //!< allocated
                heap_type  nheap; //!< node head
                node_type *root;
            public:
                code_pool  cache;
            };


            //! base class for codec
            class codec_type : public q_codec
            {
            public:
                virtual ~codec_type() throw();

            protected:
                explicit codec_type();
                tree_type  tree;
                alphabet   alpha;
                ios::bitio bio;
                void       cleanup() throw(); //! alpha, bio, tree...
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(codec_type);
            };

            class encoder : public codec_type
            {
            public:
                explicit encoder();
                virtual ~encoder() throw();

                virtual void reset() throw();
                virtual void write(char C);
                virtual void flush();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(encoder);
            };



        };


    }
}


#endif
