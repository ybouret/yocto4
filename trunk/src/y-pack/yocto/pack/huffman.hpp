#ifndef YOCTO_PACK_HUFFMAN_INCLUDED
#define YOCTO_PACK_HUFFMAN_INCLUDED 1


#include "yocto/ordered/heap.hpp"
#include "yocto/core/list.hpp"
#include "yocto/string.hpp"
#include "yocto/ios/bitio.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto 
{
    
    namespace packing 
    {
        
        struct Huffman
        {
            
            static const size_t ALPHABET_NUM = 256;              //!< possible bytes
            static const size_t NUM_CONTROLS = 2;                //!< +NYT, +END
            static const size_t ALPHABET_MAX = ALPHABET_NUM + NUM_CONTROLS; 
            static const size_t NODES_MAX    = 2*ALPHABET_MAX;   //!< Max #nodes in tree
            static const int    NYT          = -1;               //!< tag for NYT
            static const int    END          = -2;               //!< tag for END 
            static const int    INSIDE       = -3;               //!< tag for inside
            static const size_t NYT_INDEX    = ALPHABET_NUM;     //!< 
            static const size_t END_INDEX    = NYT_INDEX+1;      //!<
            
            typedef size_t      FreqType;
            typedef uint32_t    CodeType;
            static const size_t CodeBits = sizeof(CodeType) << 3;
            
            struct Node
            {
                Node      *left;   //!< has highest frequency
                Node      *right;  //!< has lowest  frequency
                FreqType   freq;   //!< cumulative  frequency
                Node      *next;   //!< for alphabet
                Node      *prev;   //!< for alphabet
                size_t     bits;   //!< #bits in code
                CodeType   code;   //!< code
                int        ch;     //!< character
                
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
                
                void       viz( ios::ostream &fp ) const;
                void       encode() throw();
                void       display( ios::ostream &fp ) const;
                void       emit( ios::bitio &out ) const;
            };
            
            typedef core::list_of<Node>           List;
            typedef heap<Node,Node::Comparator>   Heap;
            
            
            enum DecodeStatus
            {
                DecodeSuccess,
                DecodePending,
                DecodeFlushed
            };
            
            struct DecodeHandle
            {
                Node *node;
                int   flag;
            };
            
            class Tree
            {
            public:
                explicit Tree();
                virtual ~Tree() throw();
                
                void initialize() throw();
                void build_tree() throw();
                
                void graphviz( const string &graphName, ios::ostream &fp ) const;
                void display( ios::ostream &fp ) const;
                
                //! encode the byte b
                void encode( ios::bitio &out, uint8_t b );
                void flush( ios::bitio &out );
                
                //! initialize decoding
                /**
                 void *context = NULL;
                 decode_init( &context );
                 */
                void decode_init( DecodeHandle &handle ) throw();
                DecodeStatus decode( DecodeHandle &handle, ios::bitio &in, char &C );
                
            private:
                Node        *root;      //!< tree root
                List         alphabet;  //!< current #symbols
                Heap         prioQ;     //!< to build tree
                size_t       num_nodes; //!< for memory
                Node        *nodes;     //!< all the nodes 
                Node        *nyt;       //!< nodes + NYT_INDEX
                Node        *end;       //!< nodes + END_INDEX
                const size_t bytes;     //!< num_nodes * sizeof(Node)

                void update( uint8_t b ) throw(); //! update tree
                void rescale() throw(); //!< rescale frequencies
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(Tree);
                DecodeStatus decode_sym( DecodeHandle &handle, ios::bitio &in, char &C );
                DecodeStatus decode_any( DecodeHandle &handle, ios::bitio &in, char &C );
            };
            
        };
        
        
    }
}

#endif
