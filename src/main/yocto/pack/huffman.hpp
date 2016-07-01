#ifndef YOCTO_PACK_HUFFMAN_INCLUDED
#define YOCTO_PACK_HUFFMAN_INCLUDED 1

#include "yocto/ios/codec.hpp"
#include "yocto/ordered/heap.hpp"

namespace yocto
{
    namespace pack
    {


        struct huffman
        {

            typedef int      char_type; //!< internal representation of chars
            typedef size_t   freq_type; //!< frequency
            typedef unsigned code_type; //!< internal code type

            static const size_t    max_bytes = 256; //!< regular bytes
            static const size_t    max_ctrls = 2;   //!< control code
            static const size_t    max_items = max_bytes+max_ctrls;
            static const char_type NYT       = max_bytes;
            static const char_type END       = NYT+1;
            static const char_type INS       = -1;

            //! one item of alphabet
            class item_type
            {
            public:
                const char_type data; //!< identity
                freq_type       freq; //!< frequency
                char_type       code; //!< current code
                size_t          bits; //!< current bits
                
                item_type(const char_type data) throw();

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(item_type);
                ~item_type() throw();
            };
            static const char *item_text(const char_type ch) throw();

            //! the alphabet, manage items and their frequencies
            class alphabet
            {
            public:
                alphabet();
                ~alphabet() throw();
                void initialize() throw(); //!< assign code/bits/freq
                void rescale() throw();    //!< scale down frequencies

                void update(const char c) throw();
                void enable_end() throw();

                item_type & operator[](const size_t i) throw();


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(alphabet);
                size_t     size;  //!< num items
                size_t     itnum; //!< for memory
                item_type *items; //!< for items
                void enable_nyt() throw();
            };

            static const size_t max_nodes = 2 * max_items - 1;
            static const size_t max_bits  = 16;

            class node_type
            {
            public:
                node_type      *left;
                node_type      *right;
                const freq_type freq;
                const char_type data;

                node_type(const char_type ch, const freq_type fr) throw();

                void viz(ios::ostream &fp) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
                ~node_type() throw();
            };


            struct node_comparator
            {
                inline int operator()(const node_type &lhs, const node_type &rhs) throw()
                {
                    return __compare_decreasing(lhs.freq,rhs.freq);
                }
            };

            typedef heap<node_type,node_comparator> heap_type;

            class tree_type
            {
            public:
                tree_type();
                ~tree_type() throw();

                void build_for( alphabet &alpha );
                void graphviz(const string &filename) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree_type);
                node_type *root;
                size_t     count; //!< for memory
                node_type *nodes; //!< allocated
                heap_type  nheap; //!< node head
            };


        };


    }
}


#endif
