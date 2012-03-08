#ifndef YOCTO_PACK_HUFF_INCLUDED
#define YOCTO_PACK_HUFF_INCLUDED 1

#include "yocto/ordered/heap.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    
    namespace packing
    {
        
        struct huffman
        {
            typedef size_t freq_t;
            struct node_t
            {
                node_t *parent; //!< parent
                node_t *left;   //!< has highest frequency
                node_t *right;  //!< has lowest  frequency
                freq_t  freq;   //!< cumulative  frequency
                node_t *next;   //!< for pool/list
                node_t *prev;   //!< for pool/list
            };
            
            typedef core::list_of<node_t> list_t;
            class tree
            {
            public:
                virtual ~tree() throw();
                explicit tree();
                
                void reset() throw();
                
            private:
                node_t *root;
                size_t  count;
                node_t *nodes;
                list_t  alphabet;
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree);
            };
            
        };
        
    }
    
}

#endif
