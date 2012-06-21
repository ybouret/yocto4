#include "yocto/pack/huffman.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

#include <iostream>

namespace yocto 
{
    
    namespace packing
    {
        
        Huffman:: Core:: Core( Heap &prioQ ) :
        root(NULL),
        alphabet(),
        num_nodes(NODES_MAX),
        nodes( memory::kind<memory::global>::acquire_as<Node>( num_nodes ) ),
        nyt( nodes + NYT_INDEX ),
        end( nodes + END_INDEX )
        {
            initialize(prioQ);
        }
        
        
        Huffman:: Core:: ~Core() throw()
        {
            alphabet.reset();
            memory::kind<memory::global>::release_as<Node>( nodes, num_nodes );
            nyt  = NULL;
            end  = NULL;
            root = NULL;
        }
        
        void Huffman:: Core:: initialize( Heap &prioQ ) throw()
        {
            alphabet.reset();
            memset( nodes, 0, num_nodes * sizeof(Node) );
            
            //------------------------------------------------------------------
            //-- prepare possible symbols
            //------------------------------------------------------------------
            for( int i=0; i < ALPHABET_NUM; ++i )
            {
                Node *node = nodes+i;
                node->ch   = i;
                node->bits = 8;
                node->code = i;
            }
            
            //------------------------------------------------------------------
            //-- prepare control nodes
            //------------------------------------------------------------------
            nyt->ch = NYT;
            end->ch = END;
            
            //------------------------------------------------------------------
            //-- prepare inside nodes
            //------------------------------------------------------------------
            for( int i=ALPHABET_MAX; i < NODES_MAX; ++i )
            {
                nodes[i].ch = INSIDE;
            }
            
            //------------------------------------------------------------------
            //-- initialize alphabet
            //------------------------------------------------------------------
            build_tree(prioQ);

        }
        
        
        bool Huffman:: Core:: try_build_tree( Heap &prioQ ) throw()
        {
            size_t idx = ALPHABET_MAX;
            
            prioQ.free();
            for( Node *node = alphabet.head; node; node=node->next )
            {
                node->bits = 0;
                prioQ.push(node);
            }
            prioQ.push(end);
            if( alphabet.size < ALPHABET_NUM )
                prioQ.push(nyt);
            assert(prioQ.size()>0);
            
            //------------------------------------------------------------------
            // Hufmman's algorithm
            //------------------------------------------------------------------
            while( prioQ.size() >= 2 )
            {
                assert(idx<NODES_MAX);
                Node *left    = prioQ.pop();
                Node *right   = prioQ.pop();
                Node *parent  = &nodes[idx++];
                parent->left  = left;
                parent->right = right;
                parent->freq  = left->freq + right->freq;
                parent->bits  = max_of(left->bits,right->bits)+1;
                if( parent->bits > RootBits )
                {
                    return false;
                }
                prioQ.push(parent);
            }
            
            root = prioQ.pop();
            return true;
        }
        
        void Huffman:: Core:: build_tree( Heap &Q ) throw()
        {
            while( ! try_build_tree(Q) )
            {
                rescale();
            }
            assert(root!=NULL);
            root->bits = 0;
            root->code = 0;
            root->encode();
        }
        
        void Huffman:: Core:: rescale() throw()
        {
            for( Node *node = alphabet.head; node; node = node->next )
            {
                node->freq = FreqDown(node->freq);
            }
        }
        
        void Huffman:: Core:: update( uint8_t b, Heap &Q ) throw()
        {
            Node *node = nodes+b;
            if( node->freq == 0 )
            {
                assert( !alphabet.owns(node) );
                alphabet.push_front(node);
            }
#if !defined (NDEBUG)
            else { assert(alphabet.owns(node)); }
#endif
            node->freq++;
            build_tree(Q);
        }
        
        
        void Huffman:: Core:: write( ios::bitio &out, uint8_t b, Heap &Q)
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
            build_tree(Q);
        }

        
        void Huffman:: Core:: flush( ios::bitio &out )
        {
            end->emit(out);
            out.fill_to_byte_with(false);
        }


        size_t Huffman:: Core:: guess_size( const Core &other ) const throw()
        {
            const List &encoding  = other.alphabet;
            assert( alphabet.size == encoding.size );
            size_t bits  = 0;
            size_t bytes = 0;
            for( const Node *cur = alphabet.head, *enc = encoding.head; cur; cur = cur->next, enc = enc->next )
            {
                assert(cur->ch==enc->ch);
                bits += cur->freq * enc->bits;
                while( bits >= 8 )
                {
                    bits -= 8;
                    ++bytes;
                }
            }
            if( bits > 0 ) ++bytes;
            return bytes;
        }
        
    }
    
}
