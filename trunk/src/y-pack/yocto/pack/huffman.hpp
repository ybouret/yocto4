#ifndef YOCTO_PACK_HUFFMAN_INCLUDED
#define YOCTO_PACK_HUFFMAN_INCLUDED 1


#include "yocto/ordered/heap.hpp"
#include "yocto/core/list.hpp"
#include "yocto/string.hpp"
#include "yocto/ios/bitio.hpp"

namespace yocto 
{
    
    namespace packing 
    {
        
        struct Huffman
        {
            
            static const size_t ALPHABET_NUM = 256;              //!< possible bytes
            static const size_t ALPHABET_MAX = ALPHABET_NUM + 2; //!< +NYT +END
            static const size_t NODES_MAX    = 2*ALPHABET_MAX;   //!< Max #nodes in tree
            static const int    NYT       = -1;                  //!< tag for NYT
            static const int    END       = -2;                  //!< tag for END 
            static const size_t NYT_INDEX = ALPHABET_NUM;
            static const size_t END_INDEX = NYT_INDEX+1;
            
            typedef size_t      FreqType;
            typedef uint32_t    CodeType;
            static const size_t CodeBits = sizeof(CodeType) << 3;
            
            struct Node
            {
                Node      *parent; //!< parent
                Node      *left;   //!< has highest frequency
                Node      *right;  //!< has lowest  frequency
                FreqType   freq;   //!< cumulative  frequency
                Node      *next;   //!< for alphabet
                Node      *prev;   //!< for alphabet
                size_t     bits;   //!< #bits in code
                CodeType   code;   //!< code
                int        ch;     //!< character
                CodeType   cbit;   //!< coding bit
                
                class Comparator
                {
                public:
                    inline  Comparator() throw() {}
                    inline ~Comparator() throw() {} 
                    inline int operator()( const Node &lhs, const Node &rhs ) throw()
                    {
                        return lhs.freq < rhs.freq ? 1 : ( rhs.freq < lhs.freq ? -1 : 0 );
                    }
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(Comparator);
                };
            };
            
            typedef core::list_of<Node>           List;
            typedef heap<Node,Node::Comparator>   Heap;
            
            
            class Tree
            {
            public:
                explicit Tree();
                virtual ~Tree() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Tree);
            };
            
        };
        
        
    }
}

#endif
