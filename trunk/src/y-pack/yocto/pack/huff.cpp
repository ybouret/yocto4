#include "yocto/pack/huff.hpp"

namespace yocto
{
    
    namespace packing
    {
        
        huffman::tree:: ~tree() throw()
        {
            reset();
            memory::kind<memory::global>::release_as<node_t>( nodes, count );
        }
        
        huffman:: tree:: tree() :
        root( NULL ),
        count( 1000 ),
        nodes( memory::kind<memory::global>::acquire_as<node_t>( count ) ),
        alphabet()
        {
            
        }
        
        
        void huffman:: tree:: reset() throw()
        {
            root = NULL;
            alphabet.reset();
        }
        
    }
    
}