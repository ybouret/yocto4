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
        Q(ALPHABET_MAX,as_capacity),
        core1( Q ),
        current( &core1 )
        {
            
        }
        
        Huffman:: Tree:: ~Tree() throw()
        {
            
        }
        
        
        void Huffman:: Tree:: initialize() throw()
        {
            current->initialize(Q);
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
        
        
        Huffman::FreqType Huffman:: FreqDown( FreqType f ) throw()
        {
            return (f>>1) | 1;
        }
        
        void Huffman:: Tree:: write( ios::bitio &out, uint8_t b )
        {
            assert(current);
            current->write(out, b, Q);
        }
        
        void Huffman:: Tree:: flush( ios::bitio &out )
        {
            assert(current);
            current->flush(out);
        }
        
        
        
    }
    
}