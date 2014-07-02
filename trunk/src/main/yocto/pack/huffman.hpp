#ifndef YOCTO_PACK_HUFFMAN_INCLUDED
#define YOCTO_PACK_HUFFMAN_INCLUDED 1


#include "yocto/pack/q-codec.hpp"
#include "yocto/ordered/heap.hpp"
#include "yocto/type/ints.hpp"
#include "yocto/ios/bitio.hpp"

#include <iosfwd>

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
                friend std::ostream & operator<<( std::ostream &, const Item &);
                
                void emit( ios::bitio &bio ) const;
                
            private:
                YOCTO_DISABLE_ASSIGN(Item);
                
            };
            
            static  const size_t   AlphabetSize = 256 + 2;
            static  const CharType NYT = 0x100;
            static  const CharType END = 0x101;
            static  const CharType INS = -1;
            typedef memory::global Memory;
            
            
            class Alphabet
            {
            public:
                explicit Alphabet();
                virtual ~Alphabet() throw();
                void reset() throw();
                const Item & operator[](CharType Ch) const throw();
                
                void increase(uint8_t b) throw();
                friend std::ostream & operator<<( std::ostream &, const Alphabet &);
                
                void rescale() throw();
                size_t chars() const throw(); //!< #active chars, excluding NYT and END
                void   check_nyt() throw();   //!< switch off NYT is chars()>=256
                
            private:
                size_t numch; //!< #active chars
                size_t count; //!< for memory
                Item  *items; //!< memory
                YOCTO_DISABLE_COPY_AND_ASSIGN(Alphabet);
            };
            
            
            class Node
            {
            public:
                Node *parent;
                Node *left;
                Node *right;
                const CharType Char;
                const FreqType Freq;
                CodeType       cbit; //!<code bit, assigned during construction
                
                Node(CharType Ch, FreqType Fr) throw();
                ~Node() throw();
                struct Comparator
                {
                    int operator()(const Node &, const Node &) throw();
                };
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Node);
            };
            
            static const size_t NodesMax = 2*AlphabetSize - 1;
            static const size_t RootBits = 16; //!< maximum 16 bits
            
            class Tree
            {
            public:
                explicit Tree();
                virtual ~Tree() throw();
                void build_for( Alphabet &alpha ) throw();
                
                const Node *get_root() const throw();
                
            private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Tree);
                Node  *root;
                heap<Node,Node::Comparator,Memory::allocator> h;
                size_t count;
                Node  *nodes;
                bool try_build_for( Alphabet &alpha ) throw();

            };
            
            //! base class for encoder/decoder
            class Codec : public q_codec
            {
            public:
                virtual ~Codec() throw();
                const Alphabet &alphabet() const throw();
                
            protected:
                explicit Codec();
                Alphabet   alpha;
                Tree       tree;
                ios::bitio bio;
                void       cleanup() throw(); //! Q, alpha, bio, tree
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Codec);
            };
            
            
            //! encoder
            class encoder : public Codec
            {
            public:
                explicit encoder();
                virtual ~encoder() throw();
                
                virtual void reset() throw();
    
                virtual void write(char C);
                virtual void flush();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(encoder);
            };
            
            //! decoder
            class decoder : public Codec
            {
            public:
                explicit decoder();
                virtual ~decoder() throw();
                
                virtual void reset() throw();
                virtual void write(char C);
                virtual void flush();
                
            private:
                enum status
                {
                    wait_for8,
                    wait_for1
                };
                void        emit(uint8_t b);
                status      flag;
                const Node *node;
                YOCTO_DISABLE_COPY_AND_ASSIGN(decoder);
            };
        };
    }
}

#endif
