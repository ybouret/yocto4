#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/atom.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		typedef lexical::atom        lexeme;
		typedef lexical::atoms       lexemes;
		
		//! lexical analyzer
		class lexer
		{
		public:
			explicit lexer() throw();
			virtual ~lexer() throw();
			
			void operator()( pattern *motif, const string &label, const lexical::ppty_t flags=0 );
			void operator()( pattern *motif, const char   *label, const lexical::ppty_t flags=0  );
			
			inline void operator()( const string &expr, const string &label, const lexical::ppty_t flags=0, const pattern_db *dict = NULL  )
			{
				(*this)( compile(expr,dict), label, flags);
			}
			
			inline void operator()( const char *expr, const string &label, const lexical::ppty_t flags=0, const pattern_db *dict = NULL  )
			{
				(*this)( compile(expr,dict), label, flags);
			}
			
			inline void operator()( const string expr, const char *label, const lexical::ppty_t flags=0, const pattern_db *dict = NULL )
			{
				(*this)( compile(expr,dict), label,flags);
			}
			
			inline void operator()( const char *expr, const char *label, const lexical::ppty_t flags=0, const pattern_db *dict = NULL )
			{
				(*this)( compile(expr,dict), label,flags);
			}
			
			//! reset all rules and cache
			void reset( source &src ) throw();
			
			//! return lexeme to cache
			void     unget( lexeme *lx ) throw();
			
			//! return lexemes to cache
			void     unget( lexemes & ) throw();
			
			//! get next lookahead in src
			/**
			 \return not found => NULL
			 if( src.peek() != NULL ) => unknown char !
			 */
			lexeme  *lookahead( source &src );
						
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
			core::list_of<lexical::rule> rules_; //!< all the possible tokens
			lexeme *consume( source &src );      //!< read next lexeme if possible
				
		public:
			lexemes                      cache; //!< already scanned lexemes
		};
		
	}
}

#endif
