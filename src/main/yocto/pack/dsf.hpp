#ifndef YOCTO_PACK_DSF_INCLUDED
#define YOCTO_PACK_DSF_INCLUDED 1

#include "yocto/ios/codec.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace pack
    {
        //! Dynamic Shannon-Fano
        struct DSF
        {
            static const size_t MaxBytes = 256;
            static const size_t NumCtrls = 2;
            static const size_t MaxItems = MaxBytes+NumCtrls;
            typedef int         CharType;
            typedef size_t      FreqType;
            typedef unsigned    CodeType;
            static  const       CharType END = 256;
            static  const       CharType NYT = 257;
            static  const       CharType INS = -1;

            class Alphabet
            {
            public:
                struct Item
                {
                    CharType Char;
                    FreqType Freq;
                    CodeType Code;
                    size_t   Bits;
                    Item   **Slot;
                };




                Alphabet();
                ~Alphabet() throw();

                const Item & operator[](const CharType ch) const throw();
                void display() const;

                void update(const char c);// throw();


                const size_t size;
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Alphabet);
                Item  ** store; //!< ranked item addresses
                size_t   count; //!< in store
                Item  *  items; //!< 0..MaxItems-1
                size_t   wlen;  //!< for memory
                void    *wksp;  //!< for memory

                void initialize() throw();
                void __check(const int line);
            };


        };
    }
}

#endif
