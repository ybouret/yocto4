#include "yocto/pack/huff.hpp"


#include <iostream>
#include <iomanip>
#include "yocto/ios/ocstream.hpp"

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
            {
                nodes[i].ch = FWD;
                nodes[i].ch = i;
            }
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
            //alphabet.push_back( &nodes[NYT_INDEX] );
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
                node_t *right = node->left  = prio.pop(); // first  smallest frequency 
                node_t *left  = node->right = prio.pop(); // second smallest frequency
                
                left->cbit  = 0; //!< code bit for left
                right->cbit = 1; //!< code bit for right
                left->parent  = right->parent = node;
                node->freq    = left->freq + right->freq;
                prio.__push( node );
            }
            
            //------------------------------------------------------------------
            // get the root
            //------------------------------------------------------------------
            root = prio.pop();
            
            //------------------------------------------------------------------
            // build the codes
            //------------------------------------------------------------------
            for( node_t *node = alphabet.tail; node; node=node->prev )
            {
                node->bits = 0;
                node->code = 0;
                const node_t *up = node;
                while ( up != root ) 
                {
                    const code_t part = (up->cbit << (node->bits++));
                    node->code |=  part;
                    up = up->parent;
                }
            }
            
        }
        
        void huffman:: tree:: show( std::ostream &os ) const
        {
            os << "----------------" << std::endl;
            heap_t &Q  = (heap_t &)prio;
            Q.free();
            for( const node_t *node = alphabet.head; node; node=node->next )
                Q.push( (node_t*)node );
            
            while( Q.size() > 0 )
            {
                node_t *node = Q.pop();
                const int C  = node->ch;
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
                os << " [@" << std::setw(6) << node->freq << "]";
                os << " : " << std::setw(3) << node->bits << " : ";
                for( size_t ibit = 1; ibit <= node->bits; ++ibit )
                {
                    if( 0 != ( node->code & ( 1 << (node->bits-ibit) ) ) )
                        os << "1";
                    else 
                        os << "0";
                    
                }
                os << std::endl;
            }
        }
        
        static inline void __out( const huffman::node_t *node, ios::ostream &fp )
        {
            assert( node );
            const int C = node->ch;
            switch( C )
            {
                case huffman::NYT:
                    fp("\"NYT\"");
                    break;
                    
                case huffman::END:
                    fp("\"END\"");
                    break;
                default:
                    if( C >= 32 && C < 127 )
                        fp("\"'%c'\"",C);
                    else 
                        fp("\"%3d\"",C);
                    break;
            }
        }
        
        static inline void __graph( const huffman::node_t *node, ios::ostream &fp )
        {
            assert(node);
            if(node->left)
            {
                __out(node,fp); fp(" -> "); __out(node->left,fp); fp(";\n");
                __graph(node->left,fp);
            }
            if(node->right)
            {
                __out(node,fp); fp(" -> "); __out(node->right,fp); fp(";\n");
                __graph(node->right,fp);
            }
            
        }
        
        void huffman:: tree:: graph( const string &filename ) const
        {
            ios::ocstream fp( filename, false );
            
            fp("digraph G {\n");
            __graph(root,fp);
            fp("}\n");
        }
        
        
        void huffman:: tree:: encode( ios::bitio &out, uint8_t C )
        {
            node_t *node = &nodes[C];
            if( node->freq <= 0 )
            {
                //==============================================================
                // got a new char
                //==============================================================
                node_t *nyt = &nodes[NYT_INDEX];
                if( alphabet.size <= 1 )
                {
                    //----------------------------------------------------------
                    //-- first char ever: register NYT
                    //----------------------------------------------------------
                    alphabet.push_back( nyt );
                }
                else 
                {
                    //----------------------------------------------------------
                    //-- a new char : emit NYT
                    //----------------------------------------------------------
                    out.push<code_t>( nyt->code, nyt->bits );
                }
                
                alphabet.push_front( node );
                
                // TODO: remove NYT when all chars are present
            }
            else
            {
                //==============================================================
                // heuristics
                //==============================================================
                assert( alphabet.owns(node) );
                alphabet.move_to_front( node );
            }
            
            //==================================================================
            // update frequency
            //==================================================================
            ++(node->freq);
            
            //==================================================================
            // emit current model
            //==================================================================
            out.push<code_t>(node->code,node->bits);
            
            //==================================================================
            // update model
            //==================================================================
            build_tree();
            
        }
        
        void huffman:: tree:: flush( ios::bitio &out )
        {
            node_t *end = &nodes[END_INDEX];
            out.push<code_t>( end->code, end->bits );
            while( out.size() & 7 ) out.push( false );
        }
        
        
    }
    
}