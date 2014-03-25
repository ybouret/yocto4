#include "yocto/pack/huffman.hpp"
#include "yocto/exception.hpp"


namespace yocto 
{
    
    namespace packing
    {
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
            for( size_t i=1;i<=bits;++i)
            {
                const CodeType the_bit = 1 << (i-1);
                if( code & the_bit ) fp("1"); else fp("0");
            }
            fp("\n");
            
        }
        
        
        void  Huffman:: Tree:: graphviz( const string &graphName, ios::ostream &fp ) const
        {
            assert(root!=NULL);
            fp("digraph %s {\n", graphName.c_str());
            fp << "rankdir=TB;\n";
            fp << "ordering=out;\n";
            root->viz(fp);
            fp << "}\n";
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
        
        
        
    }
    
}
