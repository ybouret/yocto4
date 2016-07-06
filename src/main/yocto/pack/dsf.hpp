#ifndef YOCTO_PACK_DSF_INCLUDED
#define YOCTO_PACK_DSF_INCLUDED 1

#include "yocto/ios/codec.hpp"
#include "yocto/core/pool.hpp"

namespace yocto
{
    namespace pack
    {
        //! Dynamic Shannon-Fano
        struct DSF
        {
            static const size_t  MaxBytes = 256;
            static const size_t  NumCtrls = 2;
            static const size_t  MaxItems = MaxBytes+NumCtrls;
            typedef int          CharType;
            typedef size_t       FreqType;
            typedef unsigned     CodeType;
            static  const        CharType END = 256;
            static  const        CharType NYT = 257;
            static  const size_t MaxBits      = 16;
            
            //! info about a char
            struct Item
            {
                CharType Char;
                FreqType Freq;
                CodeType Code;
                size_t   Bits;
                Item   **Slot;
            };


            class Tree;

            //! bookeeping used char, ordered by increasing frequencies
            class Alphabet
            {
            public:
                Alphabet();
                ~Alphabet() throw();

                void display() const;
                void update(const char c) throw();
                void rescale() throw();

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
                friend class Tree;
            };


            struct Node
            {
                Node  *left;
                Node  *right;
                Node  *next;
                Item **start;
                size_t count;
                typedef core::pool_of<Node> Stack;
                void viz( ios::ostream &fp ) const;
            };


            class Tree
            {
            public:
                static const size_t MaxNodes = 2*MaxItems;
                Tree();
                ~Tree() throw();

                void build_using( Alphabet &alphabet );
                void graphviz( const string &filename ) const;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Tree);
                Node          *nodes;
                Node::Stack    stack;
                size_t         wlen;
                void          *wksp;
            };


        };
    }
}

#endif
