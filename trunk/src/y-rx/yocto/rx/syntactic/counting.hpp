#ifndef YOCTO_RX_SYN_COUNTING_INCLUDED
#define YOCTO_RX_SYN_COUNTING_INCLUDED 1

#include "yocto/rx/syntactic/joker.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace syntactic
		{
		
			class counting : public joker
			{
			public:
				virtual ~counting() throw();
				const size_t min_number;
				
			protected:
				explicit counting( uint32_t t, const string &n, rule *r, size_t m );
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(counting);
				virtual  syntax::result analyze( lexer &, source &, lexemes &stk, context &ctx);
			};
		
			
			class optional : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('J','K','?',' ');
				virtual ~optional() throw();
				
				//! rule is handled in case of error
				static optional *create( const string &n, rule *r );
				static optional *create( const char   *n, rule *r );
				
			private:
				explicit optional( const string &n, rule *r );
				YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
			};
			
			class one_or_more : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('J','K','+',' ');
				virtual ~one_or_more() throw();
				
				//! rule is handled in case of error
				static one_or_more *create( const string &n, rule *r );
				static one_or_more *create( const char   *n, rule *r );
				
			private:
				explicit one_or_more( const string &n, rule *r );
				YOCTO_DISABLE_COPY_AND_ASSIGN(one_or_more);
			};
			
			
		}

		typedef syntactic::optional s_optional;
		typedef syntactic::optional s_one_ore_more;
		
	}

}
#endif

