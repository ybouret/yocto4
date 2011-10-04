
#ifndef YOCTO_RX_SYNTAX_RULE_INCLUDED
#define YOCTO_RX_SYNTAX_RULE_INCLUDED

#include "yocto/rx/lexer.hpp"
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
		
#define YOCTO_RX_SYNTAX_RULE_ARGS lexer &lxr, source &src
		
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
				
				void   withhold() throw() { ++nref_; }
				bool   liberate() throw() { assert( nref_ > 0 ); return --nref_ <= 0; }
				size_t refcount() const throw() { return nref_; }
				
				
				typedef intrusive_ptr<string,rule> ptr;
				const string & key() const throw() { return name; }
				
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS ) = 0;
				
			protected:
				explicit rule( uint32_t t, const string &n );
				
			private:
				size_t nref_;
				YOCTO_DISABLE_COPY_AND_ASSIGN(rule);
			};
			
			
		}
		
	}
}

#endif
