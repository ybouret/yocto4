#include "yocto/pack/huffman.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto 
{
    
    namespace packing
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Node viz
        //
        ////////////////////////////////////////////////////////////////////////
        static inline
        void __tag( const void *p, ios::ostream &fp )
        {
            union 
			{
				uint8_t k[sizeof(void*)];
				const void *  p;
			} alias;
			memset( &alias, 0, sizeof(alias) );
			alias.p = p;
			
			for( size_t i=0; i < sizeof(void*); ++i )
			{
				const uint8_t B = alias.k[i];
				fp.write( char('A' + ( (B>>4) & 0xf )) );
				fp.write( char('A' + (  B     & 0xf )) );
			}
            
        }
        
        
        void Huffman:: Node:: viz( ios::ostream &fp ) const
        {
            fp("\t"); __tag(this,fp);
            if( ch >= 0 )
            {
                if( ch >= 32 && ch < 127 )
                    fp(" [label=\"'%c'\"]", char(ch) );
                else 
                    fp(" [label=\"x%02x\"]", ch );
            }
            else
            {
                switch( ch )
                {
                    case NYT:
                        fp(" [label=\"NYT\"]");
                        break;
                    case END:
                        fp(" [label=\"END\"]");
                        break;
                    default:
                        fp(" [label=\"*\"]");
                }
            }
            fp(";\n");
            if(left) 
            {
                left->viz(fp);
                fp("\t"); __tag(this,fp); fp(" -> "); __tag(left,fp);
                fp("[ label=\"0\" ]");
                fp(";\n");
            }
            if(right) 
            {
                right->viz(fp);
                fp("\t"); __tag(this,fp); fp(" -> "); __tag(right,fp);
                fp("[ label=\"1\" ]");
                fp(";\n");
            }
            
        }
        
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
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // recursive tree encoding
        //
        ////////////////////////////////////////////////////////////////////////
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
                left->parent  = parent;
                right->parent = parent;
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
        
        void  Huffman:: Tree:: graphviz( const string &graphName, ios::ostream &fp ) const
        {
            assert(root!=NULL);
            fp("digraph %s {\n", graphName.c_str());
            fp.append("rankdir=TB;\n");
            fp.append("ordering=out;\n");
            root->viz(fp);
            fp("}\n");
        }
        
        
        void Huffman:: Tree:: update( uint8_t b ) throw()
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
            build_tree();
        }
        
        void Huffman::Node:: display( ios::ostream &fp ) const
        {
            if(ch>=0)
            {
                if(ch>=32 &&ch<127)
                {
                    fp("'%c' :",char(ch));
                }
                else 
                {
                    fp("x%02x :", unsigned(ch));
                }
            }
            else 
            {
                switch(ch)
                {
                    case NYT: fp("NYT :"); break;
                    case END: fp("END :"); break;
                    default:
                        break;
                }
            }
            fp(" [%4u] :", unsigned(freq));
            fp("<%2d> ", unsigned(bits));
            for( size_t i=bits;i>0;--i)
            {
                const CodeType the_bit = 1 << (i-1);
                if( code & the_bit ) fp("1"); else fp("0");
            }
            fp("\n");
            
        }
        
        
        void Huffman:: Tree:: display( ios::ostream &fp ) const
        {
            for( const Node *node = alphabet.head; node; node=node->next )
            {
                node->display(fp);
            }
            end->display(fp);
            nyt->display(fp);
        }
        
        void Huffman::Node:: emit( ios::bitio &out ) const
        {
            assert(bits>0);
            out.push<CodeType>(code,bits);
        }
        
        void Huffman:: Tree:: encode( ios::bitio &out, uint8_t b )
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
                // else first char ever, no need for NYT
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
            
            //-- update frequency
            ++(node->freq);
            
            //-- update tree
            build_tree();
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // decoding algorithm
        //
        ////////////////////////////////////////////////////////////////////////
#define Y_HUFF_DECODE_ANY (0x01) 
#define Y_HUFF_DECODE_SYM (0x02)
        
        void Huffman:: Tree:: decode_init( DecodeHandle &handle) throw()
        {
            initialize();
            assert( root != NULL );
            handle.node = root;              //!< start at top
            handle.flag = Y_HUFF_DECODE_SYM; //!< wait for the first symbol
        }
        
        Huffman::DecodeStatus Huffman::Tree:: decode_sym( DecodeHandle &handle, ios::bitio &in, char &C )
        {
            if( in.size() >= 8 )
            {
                const uint8_t b  = in.pop_full<uint8_t>();
                C = char(b);
                update(b);
                handle.flag = Y_HUFF_DECODE_ANY;
                return DecodeSuccess;
            }
            return DecodePending;
        }
        
        Huffman::DecodeStatus Huffman::Tree:: decode_any( DecodeHandle &handle, ios::bitio &in, char &C )
        {
            while( in.size() > 0 )
            {
                //==============================================================
                // walk down the tree
                //==============================================================
                const bool at_left = in.pop();
                if( at_left )
                {
                    Node *left = handle.node->left;
                    if(!left) throw  exception("corrupted input");
                    handle.node = left;
                }
                else
                {
                    Node *right = handle.node->right;
                    if(!right) throw exception("currupted input");
                    handle.node = right;
                }
                
                const int ch = handle.node->ch;
                if( ch > INSIDE )
                {
                    //==========================================================
                    // we have a leave
                    //==========================================================
                    switch( ch )
                    {
                            
                        case END: //-- was flushed
                            handle.node = root;
                            return DecodeFlushed;
                            
                            
                        case NYT: //-- a new char
                            handle.node = root;
                            handle.flag = Y_HUFF_DECODE_SYM;
                            return decode_sym(handle, in, C);
                            
                            
                        default: //-- an existing char
                            assert(ch>=0);
                            assert(ch<ALPHABET_NUM);
                            C = char(ch);
                            handle.node = root;
                            update(ch);
                            return DecodeSuccess;
                            
                    }
                    
                }
                
            }
            return DecodePending;
        }
        
        
        Huffman::DecodeStatus Huffman::Tree:: decode( DecodeHandle &handle, ios::bitio &in, char &C )
        {
            assert( handle.node != NULL );
            assert( handle.node->ch == INSIDE);
            switch( handle.flag )
            {
                case Y_HUFF_DECODE_SYM:
                    return decode_sym( handle, in, C );
                    
                case Y_HUFF_DECODE_ANY:
                    return decode_any( handle, in, C );
                    
                default:
                    break;
                    
            }
            throw exception("Huffman: invalid handle flag=%d", handle.flag ); 
        }
        
        
        
    }
    
}