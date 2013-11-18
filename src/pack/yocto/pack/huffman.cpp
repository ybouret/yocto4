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
        Huffman:: Codec:: Codec() :
        Q(ALPHABET_MAX,as_capacity),
        H(Q)
        {
            
        }
        
        Huffman:: Codec:: ~Codec() throw()
        {
            
        }
        
        
        void Huffman:: Codec:: initialize() throw()
        {
            H.initialize(Q);
        }
        
        
        Huffman::FreqType Huffman:: FreqDown( FreqType f ) throw()
        {
            return (f>>1) | 1;
        }
        
        void Huffman:: Codec:: write( ios::bitio &out, uint8_t b )
        {
            H.write(out, b, Q);
        }
        
        void Huffman:: Codec:: flush( ios::bitio &out )
        {
           H.flush(out);
        }
        
        
    }
    
}

