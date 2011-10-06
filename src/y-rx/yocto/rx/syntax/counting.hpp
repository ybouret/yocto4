#ifndef YOCTO_RX_SYN_COUNTING_INCLUDED
#define YOCTO_RX_SYN_COUNTING_INCLUDED 1

#include "yocto/rx/syntax/rule.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			
			class counting : public rule
			{
			public:
				virtual ~counting() throw();
				
			protected:
				explicit counting( uint32_t t, const string &n, int p, grammar &g, const string &m);

			private:
				rule::ptr motif;
				YOCTO_DISABLE_COPY_AND_ASSIGN(counting);
				virtual bool is_valid( size_t count) const throw() = 0;
				virtual syntax_result match( YOCTO_RX_SYNTAX_RULE_ARGS );
			};
			
			
			class optional : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('#','0','|','1');
				virtual ~optional() throw();
				explicit optional( const string &n, int p, grammar &g, const string &m);
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
				virtual bool is_valid(size_t count) const throw();
			};
			
			
			class any_count : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('#','>', '=','0');
				virtual ~any_count() throw();
				explicit any_count( const string &n, int p, grammar &g, const string &m);
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(any_count);
				virtual bool is_valid(size_t count) const throw();
			};
			
			
			class one_or_more : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('#','>','=','1');
				virtual ~one_or_more() throw();
				explicit one_or_more( const string &n, int p, grammar &g, const string &m);
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(one_or_more);
				virtual bool is_valid(size_t count) const throw();
			};
			
		}

				
		
	}
	
}

#endif
