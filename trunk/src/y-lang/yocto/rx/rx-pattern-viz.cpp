#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#define snprintf _snprintf
#endif

#include "yocto/rx/pattern.hpp"
#include "yocto/ios/ostream.hpp"

#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"
#include "yocto/code/utils.hpp"

#include <cstdio>



namespace yocto
{
	
	namespace regex
	{
		
		void pattern:: graphviz(  ios::ostream &os, const string &graph_name ) const
		{
			os << "digraph " << graph_name << " {\n";
			viz( os );
			os << "}\n";
		}
		
		
		void  show_tag( ios::ostream &os, const void *self )
		{
			union 
			{
				uint8_t k[sizeof(void*)];
				void *  p;
			} alias;
			memset( &alias, 0, sizeof(alias) );
			alias.p = (void*)self;
			
			for( size_t i=0; i < sizeof(void*); ++i )
			{
				const uint8_t B = alias.k[i];
				os.write( char('A' + ( (B>>4) & 0xf )) );
				os.write( char('A' + (  B     & 0xf )) );
			}
		}
		
		void pattern:: tag( ios::ostream &os ) const
		{
			show_tag(os,this);
		}
		
		void pattern:: out( ios::ostream &os, char c )
		{
			uint8_t C = c;
			if( C >= 32 && C < 127 )
			{
				switch( C )
				{
					case '\"': os.write('\\'); os.write( '\"'); break;
					case '\\': os.write('\\'); os.write( '\\'); break;
					default:
						os.write( C );
				}
			}
			else 
			{
				char buffer[8];
				snprintf( buffer, sizeof(buffer)-1, "0x%02x", C );
				os << buffer;
			}
			
		}
		
		namespace basic
		{
			////////////////////////////////////////////////////////////////////
			void    any1:: viz( ios::ostream &os ) const
			{
				tag(os); 
				os << " [ label=\"any1\"];\n";
			}
			
			void single:: viz( ios::ostream &os ) const
			{
				tag(os); 
				os<< " [ label=\"'";
				out(os,value );
				os << "'\"];\n";
			}
			
			void range:: viz( ios::ostream &os ) const
			{
				tag(os);
				os<<"[ label=\"['";
				out(os,lower);
				os<<"'-'";
				out(os,upper);
				os<<"']\" ];\n";
				
			}
			
			void within:: viz( ios::ostream &os ) const
			{
				tag(os);
				os<<"[ label=\"<";
				for( symbols::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
				{
					out(os, *i );
				}
				os<<">\" ];\n";
			}
			
			
		}
		
		
		namespace logical
		{
			
			static inline void viz_op( ios::ostream &os, const p_list &op )
			{
				for( const pattern *p = op.head; p; p=p->next )
				{
					p->viz(os);
				}
			}
			
			static inline void viz_op_link( ios::ostream &os, const p_list &op, const pattern &from )
			{
				for( const pattern *p = op.head; p; p=p->next )
				{
					from.tag(os); os<<" -> "; p->tag(os); os<<";\n";
				}
			}
			
			////////////////////////////////////////////////////////////////////
			void AND:: viz( ios::ostream &os ) const
			{
				viz_op( os, operands );
				tag(os); os<< " [ label=\"AND\", shape=house ]";
				os << ";\n";
				viz_op_link( os, operands, *this );
			}
			
			
			void OR:: viz( ios::ostream &os ) const
			{
				viz_op( os, operands );
				tag(os); os<< " [ label=\"OR\", shape=house ]";
				os<<";\n";
				viz_op_link( os, operands, *this );
			}
			
			void NONE:: viz( ios::ostream &os ) const
			{
				viz_op( os, operands );
				tag(os); os<< " [ label=\"NONE\", shape=house ]";
				os << ";\n";
				viz_op_link( os, operands, *this );
			}
			
			
		}
		
		
	}
}
