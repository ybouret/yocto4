#ifndef YOCTO_PACK_HUFF_INCLUDED
#define YOCTO_PACK_HUFF_INCLUDED 1

#include "yocto/ordered/heap.hpp"
#include "yocto/core/list.hpp"
#include "yocto/string.hpp"
#include "yocto/ios/bitio.hpp"

#include <iosfwd>

namespace yocto
{
    
    namespace packing
    {
        
        struct huffman
        {
            static const size_t ALPHA_NUM = 256;
            static const size_t ALPHA_MAX = ALPHA_NUM + 2; //!< 8 bits + END + NYT
            static const size_t COUNT_MAX = 2*ALPHA_MAX;
            static const int    NYT       = -1;            //!< tag for NYT
            static const int    END       = -2;            //!< tag for END 
            static const int    FWD       = -3;            //!< tag for FWD
            static const size_t NYT_INDEX = ALPHA_NUM;
            static const size_t END_INDEX = NYT_INDEX+1;
            typedef size_t      freq_t;
            typedef uint32_t    code_t;
            
          
            
            struct node_t
            {
                node_t  *parent; //!< parent
                node_t  *left;   //!< has highest frequency
                node_t  *right;  //!< has lowest  frequency
                freq_t   freq;   //!< cumulative  frequency
                node_t  *next;   //!< for alphabet
                node_t  *prev;   //!< for alphabet
                size_t   bits;   //!< #bits in code
                code_t   code;   //!<  code
                int      ch;     //!< character
                code_t   cbit;   //!< coding bit
            };
            
            class node_comparator
            {
            public:
                explicit node_comparator() throw() {}
                virtual ~node_comparator() throw() {}
                inline int operator()( const node_t &lhs, const node_t &rhs ) throw()
                {
                    return lhs.freq < rhs.freq ? 1 : ( rhs.freq < lhs.freq ? -1 : 0 );
                }
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node_comparator);
            };
            
            
            
            typedef core::list_of<node_t>        list_t;
            typedef heap<node_t,node_comparator> heap_t;
            class tree
            {
            public:
                virtual ~tree() throw();
                explicit tree();
                
                void reset() throw();
                void show( std::ostream & ) const;
                void graph( const string &filename ) const;
                
                void    encode( ios::bitio &out, uint8_t C ); //!< emit and update
                void    flush( ios::bitio &out );             //!< send end and pad
                
                void    decode_start( void **handle ) throw();
                
                
            protected:
                heap_t       prio;     //!< priority queue
                node_t      *root;     //!< tree root
                size_t       count;    //!< #nodes, for memory
                node_t      *nodes;    //!< contiguous nodes
                size_t       msize;    //!< memory occupied by nodes
                list_t       alphabet; //!< currently active nodes
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(tree);
                void initialize() throw(); //!< assign initial frequencies and codes
                void build_tree() throw(); //!< build the tree
            };
            
                      
            
        };
        
    }
    
}

#endif
