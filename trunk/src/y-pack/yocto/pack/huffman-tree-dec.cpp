#include "yocto/pack/huffman.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto 
{
    
    namespace packing
    {
       
        
        
        
#define Y_HUFF_DECODE_ANY (0x01) 
#define Y_HUFF_DECODE_SYM (0x02)
        
        void Huffman:: Tree:: decode_init( DecodeHandle &handle) throw()
        {
            initialize();
            assert( root != NULL );
            handle.node = root;              //!< start at top
            handle.flag = Y_HUFF_DECODE_SYM; //!< wait for the first symbol
        }
        
        Huffman::DecodeStatus Huffman::Tree:: decode_sym( DecodeHandle &handle, ios::bitio &in, char &C )
        {
            assert( handle.node == root );
            if( in.size() >= 8 )
            {
                //ios::ocstream fp( ios::cstderr );
                const uint8_t b  = in.pop_full<uint8_t>();
                C = char(b);
                //fp("read : "); nodes[b].display(fp);
                update(b);
                handle.node = root;
                handle.flag = Y_HUFF_DECODE_ANY;
                return DecodeSuccess;
            }
            else
                return DecodePending;
        }
        
        Huffman::DecodeStatus Huffman::Tree:: decode_any( DecodeHandle &handle, ios::bitio &in, char &C )
        {
            while( in.size() > 0 )
            {
                //==============================================================
                // walk down the tree
                //==============================================================
                //ios::ocstream fp( ios::cstderr );
                
                
                const bool at_right = in.pop();
      
                
                if( at_right )
                {
                    //fp("[1]\n");
                    Node *right = handle.node->right;
                    if(!right) throw exception("corrupted input");
                    handle.node = right;
                }
                else
                {
                    //fp("[0]\n");
                    Node *left = handle.node->left;
                    if(!left) throw  exception("corrupted input");
                    handle.node = left;
                }
                
                const int ch = handle.node->ch;
                //fp("read : "); handle.node->display(fp);
                if( ch > INSIDE )
                {
                    //==========================================================
                    // we have a leave
                    //==========================================================
                    switch( ch )
                    {
                            
                        case END: //-- was flushed
                            handle.node = root;
                            return DecodeFlushed;
                            
                            
                        case NYT: //-- a new char
                            handle.node = root;
                            handle.flag = Y_HUFF_DECODE_SYM;
                            return decode_sym(handle, in, C);
                            
                            
                        default: //-- an existing char
                            assert(ch>=0);
                            assert(ch<ALPHABET_NUM);
                            C = char(ch);
                            update(ch);
                            handle.node = root;
                            return DecodeSuccess;
                    }
                    
                }
                
            }
            return DecodePending;
        }
        
        
        Huffman::DecodeStatus Huffman::Tree:: decode( DecodeHandle &handle, ios::bitio &in, char &C )
        {
            assert( handle.node != NULL );
            assert( handle.node->ch == INSIDE);
            switch( handle.flag )
            {
                case Y_HUFF_DECODE_SYM:
                    return decode_sym( handle, in, C );
                    
                case Y_HUFF_DECODE_ANY:
                    return decode_any( handle, in, C );
                    
                default:
                    break;
                    
            }
            throw exception("Huffman: invalid handle flag=%d", handle.flag ); 
        }
        
    }
    
}
