#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/rule.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
	
	namespace regex
	{
	
		class lexeme : public token
		{
		public:
			lexeme       *next;
			lexeme       *prev;
			const string &label;
			
			explicit lexeme( const token &tk, const string &id ) throw();
			virtual ~lexeme() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(lexeme);
		};
		
		class lexer
		{
		public:
			explicit lexer() throw();
			virtual ~lexer() throw();
			
			void operator()( pattern *motif, const string &label );
			void operator()( pattern *motif, const char   *label );
			
			inline void operator()( const string &expr, const string &label, const pattern_db *dict = NULL )
			{
				(*this)( compile(expr,dict), label );
			}
			
			inline void operator()( const char *expr, const string &label, const pattern_db *dict = NULL )
			{
				(*this)( compile(expr,dict), label );
			}
			
			inline void operator()( const string expr, const char *label, const pattern_db *dict = NULL )
			{
				(*this)( compile(expr,dict), label );
			}
			
			inline void operator()( const char *expr, const char *label, const pattern_db *dict = NULL )
			{
				(*this)( compile(expr,dict), label );
			}
			
			//! reset all rules
			void reset( source &src ) throw();
			
			//! find next lexeme in src
			/**
			 
				\return not found => NULL
			 */
			lexeme * operator()( source &src );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
			core::list_of<lexical::rule> rules;
			
		};
		
	}
}

#endif
