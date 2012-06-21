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
        H(Q)
        {
            
        }
        
        Huffman:: Tree:: ~Tree() throw()
        {
            
        }
        
        
        void Huffman:: Tree:: initialize() throw()
        {
            H.initialize(Q);
        }
        
        
        Huffman::FreqType Huffman:: FreqDown( FreqType f ) throw()
        {
            return (f>>1) | 1;
        }
        
        void Huffman:: Tree:: write( ios::bitio &out, uint8_t b )
        {
            H.write(out, b, Q);
        }
        
        void Huffman:: Tree:: flush( ios::bitio &out )
        {
           H.flush(out);
        }
        
#if 0
        void Huffman:: Tree:: check() throw()
        {
            const size_t ncurr = current->guess_size( *current );
            control->alphabet.reset();
            for( const Node *node = current->alphabet.head; node; node=node->next )
            {
                assert(node->ch>=0);
                assert(node->ch<ALPHABET_NUM);
                Node *sub  = control->nodes + node->ch;
                sub->freq  = FreqDown(node->freq);
                sub->next  = sub->prev = NULL;
                control->alphabet.push_back(sub);
            }
            control->build_tree(Q);
            const size_t nctrl = control->guess_size( *control );
            // std::cerr << "current= " << ncurr << "/ guess=" << 2*nctrl << std::endl;
            if(2*nctrl<ncurr) cswap<Core *>(control,current);
        }
#endif
        
    }
    
}