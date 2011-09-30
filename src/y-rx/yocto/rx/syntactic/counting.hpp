#ifndef YOCTO_RX_SYN_COUNTING_INCLUDED
#define YOCTO_RX_SYN_COUNTING_INCLUDED 1

#include "yocto/rx/syntactic/joker.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace syntactic
		{
			
			//! counting a rule
			class counting : public joker
			{
			public:
				virtual ~counting() throw();
				
				
			protected:
				explicit counting( uint32_t t, const string &n, rule *r);
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(counting);
				virtual  syntax::result match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS );
				virtual  bool is_valid( size_t count ) const throw() = 0;
			};
			
			//! accept any count
			class any_count : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('J','K','*',' ');
				virtual ~any_count() throw();
				
				//! rule is handled in case of error
				static any_count *create( const string &n, rule *r );
				static any_count *create( const char   *n, rule *r );
				virtual rule *clone() const;
				
			private:
				explicit any_count( const string &n, rule *r );
				YOCTO_DISABLE_COPY_AND_ASSIGN(any_count);
				virtual  bool is_valid( size_t count ) const throw(); //!<  >=0 
			};
			
			
			//! accept 0 or 1 count
			class optional : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('J','K','?',' ');
				virtual ~optional() throw();
				
				//! rule is handled in case of error
				static optional *create( const string &n, rule *r );
				static optional *create( const char   *n, rule *r );
				virtual rule *clone() const;
				
			private:
				explicit optional( const string &n, rule *r );
				YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
				virtual  bool is_valid( size_t count ) const throw(); //!< 0 or 1
			};
			
			//! accept 1 ore more count
			class one_or_more : public counting
			{
			public:
				static const uint32_t ID = YOCTO_FOURCC('J','K','+',' ');
				virtual ~one_or_more() throw();
				
				//! rule is handled in case of error
				static one_or_more *create( const string &n, rule *r );
				static one_or_more *create( const char   *n, rule *r );
				virtual rule *clone() const;
				
			private:
				explicit one_or_more( const string &n, rule *r );
				YOCTO_DISABLE_COPY_AND_ASSIGN(one_or_more);
				virtual  bool is_valid( size_t count ) const throw(); //!< >=1
			};
			
			
		}
		
		typedef syntactic::optional     s_optional;
		typedef syntactic::one_or_more  s_one_ore_more;
		typedef syntactic::any_count    s_any_count;
	}
	
}
#endif

