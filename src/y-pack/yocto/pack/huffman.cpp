#include "yocto/pack/huffman.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

#include <iostream>

namespace yocto 
{
    
    namespace packing
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // Nodes internal ops
        //
        ////////////////////////////////////////////////////////////////////////
        
        void Huffman::Node:: emit( ios::bitio &out ) const
        {
            assert(bits>0);
            out.push<CodeType>(code,bits);
        }

        void Huffman::Node:: encode() throw()
        {
            const size_t child_bits = bits+1;
            
            if( left )
            {
                left->bits = child_bits;
                left->code = code;
                left->encode();
            }
            
            if( right )
            {
                right->bits = child_bits;
                right->code = code | (1<<bits);
                right->encode();
            }
        }

        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Tree Initialization
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman:: Tree:: Tree() :
        prioQ(ALPHABET_MAX,as_capacity),
        core1( prioQ ),
        current( &core1 )
        {
            
        }
        
        Huffman:: Tree:: ~Tree() throw()
        {
            
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Tree initialization
        //
        ////////////////////////////////////////////////////////////////////////
        void Huffman:: Tree:: initialize() throw()
        {
            current->initialize(prioQ);
        }
        
               
        
        
        
        Huffman::Node * Huffman:: BuildTree( List &alphabet, Node *nyt, Node *end, Heap &prioQ, Node *nodes ) throw()
        {
            size_t idx = ALPHABET_MAX;
            
            prioQ.free();
            for( Node *node = alphabet.head; node; node=node->next )
            {
                node->bits = 0;
                prioQ.push(node);
            }
            prioQ.push(end);
            if( alphabet.size < ALPHABET_NUM )
                prioQ.push(nyt);
            assert(prioQ.size()>0);
            
            //------------------------------------------------------------------
            // Hufmman's algorithm
            //------------------------------------------------------------------
            while( prioQ.size() >= 2 )
            {
                assert(idx<NODES_MAX);
                Node *left    = prioQ.pop();
                Node *right   = prioQ.pop();
                Node *parent  = &nodes[idx++];
                parent->left  = left;
                parent->right = right;
                parent->freq  = left->freq + right->freq;
                parent->bits  = max_of(left->bits,right->bits)+1;
                if( parent->bits > 32 )
                {
                    return NULL;
                }
                prioQ.push(parent);
            }
            
            return prioQ.pop();
            
        }
        
        void  Huffman:: MakeCodes(Node *root) throw()
        {
            assert(root!=NULL);
            root->bits = 0;
            root->code = 0;
            root->encode();
        }
        
        size_t Huffman:: GuessSize( const List &alphabet, const List &encoding ) throw()
        {
            assert( alphabet.size == encoding.size );
            size_t bits = 0;
            size_t bytes = 0;
            for( const Node *cur = alphabet.head, *enc = encoding.head; cur; cur = cur->next, enc = enc->next )
            {
                bits += cur->freq * enc->bits;
                while( bits >= 8 )
                {
                    bits -= 8;
                    ++bytes;
                }
            }
            return bytes;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Build the huffman tree
        //
        ////////////////////////////////////////////////////////////////////////
        void Huffman:: Tree:: build_tree() throw()
        {
            current->build_tree(prioQ);            
        }
        
        
        Huffman::FreqType Huffman:: FreqDown( FreqType f ) throw()
        {
            return (f>>1) | 1;
        }
        
      
        
        
        
    }
    
}