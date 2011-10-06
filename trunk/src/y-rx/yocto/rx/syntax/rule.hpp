
#ifndef YOCTO_RX_SYNTAX_RULE_INCLUDED
#define YOCTO_RX_SYNTAX_RULE_INCLUDED

#include "yocto/rx/syntax/c-node.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
	namespace regex
	{
		
		enum syntax_result
		{
			syntax_success,    //!< alright...
			syntax_nothing,    //!< met end of input
			syntax_unexpected, //!< got a valid lexeme, not expected
		};
	
		
		enum node_ppty
		{
			node_certain = 0x0001, //!< univocal terminal => drop chars
			node_useless = 0x0002, //!< syntax only terminal => drop node
			node_fusion  = 0x0100, //!< fusion content with parent children
		};
				
#define YOCTO_RX_SYNTAX_RULE_ARGS lexer &lxr, source &src, c_node * &tree
		
		class grammar;
		
		namespace syntax
		{
			typedef imported::exception exception;
						
			//! generic rule
			class rule : public object
			{
			public:
				
				virtual ~rule() throw();
				
				const uint32_t type;
				const string   name;
				const int      ppty;
				
				void   withhold() throw();
				bool   liberate() throw();			
				size_t refcount() const throw();				
				
				typedef intrusive_ptr<string,rule> ptr;
				const string & key() const throw() { return name; }
				
				//! todo: add a rule ** to get error context
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS ) = 0;
				
			protected:
				explicit rule( uint32_t t, const string &n, int p, grammar &g);
				grammar &G;
				
			private:
				size_t nref_;
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
				
			
			};
			
			
		}
		
	}
}

#endif
