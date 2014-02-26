#ifndef YOCTO_PACK_HUFFMAN_INCLUDED
#define YOCTO_PACK_HUFFMAN_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/type/ints.hpp"

namespace yocto
{
    namespace pack
    {
        
        struct Huffman
        {
            typedef int32_t        CharType;
            typedef uint32_t       FreqType;
            typedef uint32_t       CodeType;
            typedef uint32_t       BitsType;
            static  const FreqType FreqMax = limit_of<FreqType>::maximum;
            static  const size_t   AlphabetSize = 256 + 2;
            static  const CharType NYT = 0x100;
            static  const CharType END = 0x101;
            static  const CharType INS = -1;
            typedef memory::global Allocator;

            class Item
            {
            public:
                const CharType Char;
                FreqType       Freq;
                CodeType       Code;
                BitsType       Bits;
                Item(CharType Ch) throw();
                ~Item() throw();
                Item( const Item &other ) throw();
                
            private:
                YOCTO_DISABLE_ASSIGN(Item);
                
            };
            
            class Alphabet
            {
            public:
                explicit Alphabet();
                virtual ~Alphabet() throw();
                void reset() throw();
                Item &       operator[](CharType Ch) throw();
                const Item & operator[](CharType Ch) const throw();
                
            private:
                size_t count;
                Item  *items;
                YOCTO_DISABLE_COPY_AND_ASSIGN(Alphabet);
            };
            
            
        };
    }
}

#endif

