#ifndef YOCTO_RX_SYN_RULE_INCLUDED
#define YOCTO_RX_SYN_RULE_INCLUDED 1

#include "yocto/rx/syntactic/s-node.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/exceptions.hpp"
//#include "yocto/intrusive-ptr.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		struct syntax
		{
			enum result
			{
				success       = 0x00, //!< normal return
				nothing       = 0x01, //!< end of source
				unexpected    = 0x02  //!< a valid lexeme, unexpected here
			};
			static const char *get_info( syntax::result ) throw();
		};
		
		
		namespace syntactic
		{
			typedef yocto::imported::exception exception;
			
			class s_node;
			
#define YOCTO_RX_SYNTAX_RULE_MATCH_ARGS lexer &lxr, source &src, s_node * &tree
			
			class rule : public object
			{
			public:
				const uint32_t type;   //!< for specific hebavior
				const string   name;   //!< for identification
				rule          *next;
				rule          *prev;
				const rule    *parent; //!< for parsing status
				
				virtual ~rule() throw();
				
				virtual syntax::result match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS ) = 0;
				
				//! append calling tree to exception
				void unwind( exception & ) const throw();
				
				virtual rule *clone() const = 0;
				
				std::ostream &space( std::ostream & ) const;
				
			protected:
				explicit rule( uint32_t t, const string &id );
				void check( const s_node *node ) const;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
				
			};
			
			typedef core::meta_list<rule> rules;
			
		}
		
		typedef syntactic::rule s_rule;
	}
}

#endif
