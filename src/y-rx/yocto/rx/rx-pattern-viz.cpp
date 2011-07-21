#include "yocto/rx/pattern.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		void pattern:: graphviz(  ios::ostream &os, const string &graph_name ) const
		{
			
			os.append("digraph ");
			os.append( graph_name );
			os.append(" {\n");
			viz( os );
			os.append("}\n");
			
		}
		
		void pattern:: tag( ios::ostream &os ) const
		{
			char buffer[32];
			snprintf( buffer, sizeof(buffer)-1, "%p", this);
			os.append( buffer );
		}
		
		void pattern:: out( ios::ostream &os, char c )
		{
			uint8_t C = c;
			if( C >= 32 && C < 127 )
			{
				os.write( C );
			}
			else 
			{
				char buffer[8];
				snprintf( buffer, sizeof(buffer)-1, "0x%02x", C );
				os.append( buffer );
			}

		}
				
		namespace basic
		{
			////////////////////////////////////////////////////////////////////
			void    any1:: viz( ios::ostream &os ) const
			{
				tag(os); 
				os.append("[ label=\"any1\"];\n");
			}
			
			void single:: viz( ios::ostream &os ) const
			{
				tag(os); 
				os.append("[ label=\"'");
				out(os,value );
				os.append("\"];\n");
			}
			
			void range:: viz( ios::ostream &os ) const
			{
				tag(os);
				os.append("[ label=\"[");
				
				os.append("]\" ];\n");
				
			}
			
			void within:: viz( ios::ostream &os ) const
			{
				tag(os);
				os.append("[ label=\"<");
				
				os.append(">\" ];\n");
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
			
			////////////////////////////////////////////////////////////////////
			void AND:: viz( ios::ostream &os ) const
			{
				viz_op( os, operands );
				tag(os);
				os.append(";\n");
			}
			
			
			void OR:: viz( ios::ostream &os ) const
			{
				viz_op( os, operands );
				tag(os);
				os.append(";\n");
			}
			
			void NONE:: viz( ios::ostream &os ) const
			{
				viz_op( os, operands );
				tag(os);
				os.append(";\n");
			}
			
			
		}
		
		
	}
}
