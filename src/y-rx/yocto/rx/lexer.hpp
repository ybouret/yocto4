#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/atom.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
	
	namespace regex
	{
	
		typedef lexical::atom  lexeme;
		typedef lexical::atoms lexemes;
		
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
			
			//! reset all rules and cache
			void reset( source &src ) throw();
			
			//! find next lexeme in src
			/**
				\return not found => NULL
				if( src.peek() != NULL ) => unknowm char !
			 */
			lexeme * get( source &src );

			//! return lexeme to cache
			void     unget( lexeme *lx ) throw(); 
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
			core::list_of<lexical::rule> rules_;
			lexemes                      cache_;
			lexeme *consume( source &src ); //!< read next lexeme if possible
				
		};
		
	}
}

#endif
