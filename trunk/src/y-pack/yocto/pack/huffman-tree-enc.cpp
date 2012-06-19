#include "yocto/pack/huffman.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto 
{
    
    namespace packing
    {
        
        void Huffman::Node:: emit( ios::bitio &out ) const
        {
            assert(bits>0);
            ios::ocstream fp( ios::cstderr );
            fp("emit : "); display(fp);
            out.push<CodeType>(code,bits);
        }
        
        void Huffman:: Tree:: encode( ios::bitio &out, uint8_t b )
        {
            Node *node = &nodes[b];
            assert(node->bits>0);
            if( node->freq <= 0 )
            {
                assert( !alphabet.owns(node) );
                //--------------------------------------------------------------
                // a new char
                //--------------------------------------------------------------
                if( alphabet.size > 0 )
                {
                    nyt->emit(out);
                }
                //--------------------------------------------------------------
                // else first char ever, no need for NYT
                //--------------------------------------------------------------
                node->emit(out);
                alphabet.push_front(node);
            }
            else 
            {
                //--------------------------------------------------------------
                // already encoded char
                //--------------------------------------------------------------
                node->emit(out);
            }
            
            //------------------------------------------------------------------
            //-- update frequency
            //------------------------------------------------------------------
            ++(node->freq);
            
            //------------------------------------------------------------------
            //-- update tree
            //------------------------------------------------------------------
            build_tree();
        }
        
        void Huffman:: Tree:: flush( ios::bitio &out )
        {
            end->emit(out);
            out.fill_to_byte_with(false);
        }
        
    }
    
}
