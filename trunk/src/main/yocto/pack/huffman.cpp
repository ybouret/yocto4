#include "yocto/pack/huffman.hpp"

namespace yocto
{
    namespace pack
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // Item
        //
        ////////////////////////////////////////////////////////////////////////

        Huffman::Item:: Item(CharType Ch) throw() :
        Char(Ch),
        Freq(0),
        Code(0),
        Bits(0)
        {
            
        }
        
        Huffman::Item:: ~Item() throw() {}
        
        Huffman::Item:: Item( const Item &other ) throw() :
        Char( other.Char ),
        Freq( other.Freq ),
        Code( other.Code ),
        Bits( other.Bits )
        {
            
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Alphabet
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman:: Alphabet:: Alphabet() :
        count( AlphabetSize ),
        items( memory::kind<Allocator>::acquire_as<Item>(count) )
        {
        }
        
        Huffman:: Alphabet:: ~Alphabet() throw()
        {
            memory::kind<Allocator>::release_as<Item>(items,count);
        }
        
        
        void Huffman:: Alphabet:: reset() throw()
        {
            for(CharType i=0;i< CharType(AlphabetSize); ++i)
            {
                new ( &items[i] ) Item(i);
            }
        }
        
        Huffman:: Item & Huffman:: Alphabet:: operator[](CharType Ch) throw()
        {
            assert(Ch>=0);
            assert(Ch<AlphabetSize);
            return items[Ch];
        }
        
        
        const Huffman:: Item & Huffman:: Alphabet:: operator[](CharType Ch) const  throw()
        {
            assert(Ch>=0);
            assert(Ch<AlphabetSize);
            return items[Ch];
        }


        
    }
}