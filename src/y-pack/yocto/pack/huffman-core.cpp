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
        
        Huffman:: Tree:: Tree( Heap &prioQ ) :
        root(NULL),
        alphabet(),
        num_nodes(NODES_MAX),
        nodes( memory::kind<memory::global>::acquire_as<Node>( num_nodes ) ),
        nyt( nodes + NYT_INDEX ),
        end( nodes + END_INDEX )
        {
            initialize(prioQ);
        }
        
        
        Huffman:: Tree:: ~Tree() throw()
        {
            alphabet.reset();
            memory::kind<memory::global>::release_as<Node>( nodes, num_nodes );
            nyt  = NULL;
            end  = NULL;
            root = NULL;
        }
        
        void Huffman:: Tree:: initialize( Heap &prioQ ) throw()
        {
            alphabet.reset();
            memset( nodes, 0, num_nodes * sizeof(Node) );
            
            //------------------------------------------------------------------
            //-- prepare possible symbols
            //------------------------------------------------------------------
            for( unsigned i=0; i < ALPHABET_NUM; ++i )
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
            for( unsigned i=ALPHABET_MAX; i < NODES_MAX; ++i )
            {
                nodes[i].ch = INSIDE;
            }
            
            //------------------------------------------------------------------
            //-- initialize alphabet
            //------------------------------------------------------------------
            build_tree(prioQ);
            
        }
        
        
        bool Huffman:: Tree:: try_build_tree( Heap &prioQ ) throw()
        {
            size_t idx = ALPHABET_MAX;
            
            prioQ.free();
            for( Node *node = alphabet.head; node; node=node->next )
            {
                node->bits = 0;
                prioQ.push(node);
            }
            end->bits = 0;
            prioQ.push(end);
            if( alphabet.size < ALPHABET_NUM )
            {
                nyt->bits = 0;
                prioQ.push(nyt);
            }
            
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
                parent->bits  = max_of<size_t>(left->bits,right->bits)+1;
                if( parent->bits > RootBits )
                {
                    return false;
                }
                prioQ.push(parent);
            }
            
            root = prioQ.pop();
            return true;
        }
        
        void Huffman:: Tree:: build_tree( Heap &Q ) throw()
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
        
        void Huffman:: Tree:: rescale() throw()
        {
            for( Node *node = alphabet.head; node; node = node->next )
            {
                node->freq = FreqDown(node->freq);
            }
        }
        
        void Huffman:: Tree:: update( uint8_t b, Heap &Q ) throw()
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
        
        
        void Huffman:: Tree:: write( ios::bitio &out, uint8_t b, Heap &Q)
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
        
        
        void Huffman:: Tree:: flush( ios::bitio &out )
        {
            end->emit(out);
            out.fill_to_byte_with(false);
        }
        
        
    }
    
}
