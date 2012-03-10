#include "yocto/pack/huff.hpp"
#include <iostream>
#include <iomanip>

namespace yocto
{
    
    namespace packing
    {
        
        huffman::tree:: ~tree() throw()
        {
            root = NULL;
            alphabet.reset();
            memory::kind<memory::global>::release_as<node_t>( nodes, count );
        }
        
        huffman:: tree:: tree() :
        prio( COUNT_MAX, as_capacity ),
        root(  NULL ),
        count( COUNT_MAX ),
        nodes( memory::kind<memory::global>::acquire_as<node_t>( count ) ),
        msize( count * sizeof(node_t) ),
        alphabet()
        {
            initialize();
        }
        
        
        void huffman:: tree:: reset() throw()
        {
            root = NULL;
            alphabet.reset();
            initialize();
        }
        
        void huffman:: tree:: initialize() throw()
        {
            //------------------------------------------------------------------
            // clear all data
            //------------------------------------------------------------------
            memset(nodes,0,msize);
            
            //------------------------------------------------------------------
            // prepare future internal nodes
            //------------------------------------------------------------------
            for( size_t i=ALPHA_MAX;i<count;++i )
                nodes[i].ch = FWD;
            
            //------------------------------------------------------------------
            // prepare future leaves
            //------------------------------------------------------------------
            for( size_t i=0; i < ALPHA_MAX; ++i )
            {
                node_t *node = &nodes[i];
                node->ch     = int(i);
                node->code   = code_t(i);
                node->bits   = 8;
            }
            
            //------------------------------------------------------------------
            // prepare control sequences, with zero frequencies
            //------------------------------------------------------------------
            nodes[NYT_INDEX].ch = NYT;
            nodes[END_INDEX].ch = END;
            
            //------------------------------------------------------------------
            // initial alphabet
            //------------------------------------------------------------------
            alphabet.push_back( &nodes[NYT_INDEX] );
            alphabet.push_back( &nodes[END_INDEX] );
            
            //------------------------------------------------------------------
            // first tree
            //------------------------------------------------------------------
            build_tree();
        }
        
        void huffman:: tree:: build_tree() throw()
        {
            assert( alphabet.size > 0 );
            prio.free();
            
            //------------------------------------------------------------------
            // enqueue all the alphabet
            //------------------------------------------------------------------
            for( node_t *node = alphabet.head; node; node = node->next )
            {
                node->bits = 0;
                node->code = 0;
                assert(NULL==node->left);
                assert(NULL==node->right);
                prio.__push(node);
            }
            
            //------------------------------------------------------------------
            // create the tree
            //------------------------------------------------------------------
            size_t inode = ALPHA_MAX;
            while( prio.size() > 1 )
            {
                assert(inode<COUNT_MAX);
                node_t *node  = &nodes[inode++];
                node_t *left  = node->left  = prio.pop();
                node_t *right = node->right = prio.pop();
                
                const size_t child_bits = node->bits+1;
                left->parent  = right->parent = node;
                node->freq    = left->freq + right->freq;
                left->bits    = right->bits = child_bits;
                left->code    = right->code = (node->code << 1);
                right->code  |= 1;
                prio.__push( node );
            }
            
            //------------------------------------------------------------------
            // get the root
            //------------------------------------------------------------------
            root = prio.pop();
            
        }
        
        void huffman:: tree:: show( std::ostream &os ) const
        {
            os << "----------------" << std::endl;
            for( const node_t *node = alphabet.head; node; node=node->next )
            {
                const int C = node->ch;
                switch( C )
                {
                    case NYT:
                        os << "NYT";
                        break;
                        
                    case END:
                        os << "END"; 
                        break;
                        
                    default:
                        assert( node->ch >=0 );
                        assert( node->ch < ALPHA_NUM );
                        if( C >= 32 && C < 127 )
                            os << "'" << char(C) << "'";
                        else 
                            os << std::setw(3) << C;
                        
                        break;
                }
                os << " : " << std::setw(3) << node->bits << " : ";
                for( size_t ibit = 0; ibit < node->bits; ++ibit )
                {
                    if( 0 != ( node->code & ( 1 << (ibit) ) ) )
                        os << "1";
                    else 
                        os << "0";
                    
                }
                os << std::endl;
            }
        }
        
        void huffman:: tree:: update( uint8_t C ) throw()
        {
            node_t *node = &nodes[C];
            if( node->freq > 0 )
            {
                ++(node->freq);
                assert( alphabet.owns(node) );
            }
            else
            {
                node->freq = 1;
                alphabet.push_back(node);
            }
            build_tree();
        }

        
    }
    
}