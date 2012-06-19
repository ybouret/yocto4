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
        
       
              
        ////////////////////////////////////////////////////////////////////////
        //
        // Tree Initialization
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman:: Tree:: Tree() :
        root(NULL),
        alphabet(),
        prioQ(NODES_MAX,as_capacity),
        num_nodes( NODES_MAX ),
        nodes( memory::kind<memory::global>::acquire_as<Node>(num_nodes) ),
        nyt(nodes + NYT_INDEX),
        end(nodes + END_INDEX),
        bytes( num_nodes * sizeof(Node) )
        {
            initialize();
        }
        
        Huffman:: Tree:: ~Tree() throw()
        {
            alphabet.reset();
            memory::kind<memory::global>::release_as<Node>(nodes,num_nodes);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Tree initialization
        //
        ////////////////////////////////////////////////////////////////////////
        void Huffman:: Tree:: initialize() throw()
        {
            alphabet.reset();
            memset( nodes, 0, bytes );
            
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
            build_tree();
        }
        
        void Huffman::Node:: encode() throw()
        {
            const size_t child_bits = bits+1;
            
            if( left )
            {
                left->bits = child_bits;
                left->code = code << 1;
                left->encode();
            }
            
            if( right )
            {
                right->bits = child_bits;
                right->code = (code<<1)|1;
                right->encode();
            }
        }

               
        ////////////////////////////////////////////////////////////////////////
        //
        // Build the huffman tree
        //
        ////////////////////////////////////////////////////////////////////////
        void Huffman:: Tree:: build_tree() throw()
        {
            root       = NULL;
            size_t idx = ALPHABET_MAX;
            
            //------------------------------------------------------------------
            // enqueue alphabet
            //------------------------------------------------------------------
            prioQ.free();
            for( Node *node = alphabet.head; node; node=node->next )
            {
                node->code = 0;
                node->bits = 0;
                prioQ.push(node);
            }
            prioQ.push(end);
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
                //left->parent  = parent;
                //right->parent = parent;
                parent->freq  = left->freq + right->freq;
                parent->bits  = max_of(left->bits,right->bits)+1;
                prioQ.push(parent);
            }
            
            root = prioQ.pop();
            
            //------------------------------------------------------------------
            // assign codes
            //------------------------------------------------------------------
            root->bits = 0;
            root->code = 0;
            root->encode();
            
        }
        
             
                
        
        
    }
    
}