#ifndef YOCTO_RX_PARSER_INCLUDED
#define YOCTO_RX_PARSER_INCLUDED 1

#include "yocto/rx/syntactic/rule.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
	
	namespace regex
	{
	
		
		class parser
		{
		public:
			explicit parser( syntactic::rule *root ) throw();
			virtual ~parser() throw();
			
			void restart( lexer &lxr, source &src ) throw();
			syntax::result operator()( lexer &lxr, source &src );
			
			void release() throw();
			
			void graphviz( ios::ostream &os, const string &graph_name ) const;
			
		private:
			auto_ptr<syntactic::rule> root_;
			syntactic::s_node        *node_;

			YOCTO_DISABLE_COPY_AND_ASSIGN(parser);
			virtual void on_restart() throw(); //!< to be overriden...
		};
		
	}
	
}

#endif
