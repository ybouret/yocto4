#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/atom.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		typedef lexical::atom        lexeme;   //!< a lexical atom
		typedef lexical::atoms       lexemes;  //!< a list of lexemes
		
		//! lexical analyzer
		/**
		 Base on an ordered set of name rules, following the 'flex'
		 engine behavior.
		 */
		class lexer
		{
		public:
			explicit lexer() throw();
			virtual ~lexer() throw();
			
			//! create a lexical::rule named 'label' for 'motif'
			void operator()( pattern *motif, const string &label, const lexical::action *a = NULL);
			
			//! create a lexical::rule named 'label' for 'motif'
			void operator()( pattern *motif, const char   *label, const lexical::action *a = NULL);
			
			
			inline void operator()( const string &expr, const string &label )
			{
				(*this)( compile(expr,dict_), label, NULL );
			}
			
			inline void operator()( const char *expr, const string &label )
			{
				(*this)( compile(expr,dict_), label, NULL );
			}
			
			
			inline void operator()( const string &expr, const char *label )
			{
				(*this)( compile(expr,dict_), label, NULL );
			}
			
			inline void operator()( const char *expr, const char *label )
			{
				(*this)( compile(expr,dict_), label, NULL );
			}
			
			template <typename HOST>
			inline void operator()( const string &expr, const string &label, HOST &host, bool (HOST:: *method)( YOCTO_RX_LEX_RULE_ACTION_ARGS) )
			{
				assert( method );
				const lexical::action a( &host, method );
				(*this)( compile(expr,dict_), label, &a );
			}
			
			template <typename HOST>
			inline void operator()( const string &expr, const char *label, HOST &host, bool (HOST:: *method)( YOCTO_RX_LEX_RULE_ACTION_ARGS) )
			{
				assert( method );
				const lexical::action a( &host, method );
				(*this)( compile(expr,dict_), label, &a );
			}
			
			template <typename HOST>
			inline void operator()( const char *expr, const string &label, HOST &host, bool (HOST:: *method)( YOCTO_RX_LEX_RULE_ACTION_ARGS) )
			{
				assert( method );
				const lexical::action a( &host, method );
				(*this)( compile(expr,dict_), label, &a );
			}
			
			template <typename HOST>
			inline void operator()( const char *expr, const char *label, HOST &host, bool (HOST:: *method)( YOCTO_RX_LEX_RULE_ACTION_ARGS) )
			{
				assert( method );
				const lexical::action a( &host, method );
				(*this)( compile(expr,dict_), label, &a );
			}
			
			
			
			inline bool skip( YOCTO_RX_LEX_RULE_ACTION_ARGS ) throw()
			{
				return false;
			}
			
			
			
			//! reset all rules and cache
			void reset() throw();
			
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
			
			pattern_db       &dict() ;		
			const pattern_db &dict() const;
			void           no_dict() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
			core::list_of<lexical::rule> rules_; //!< all the possible tokens
			lexeme *consume( source &src );      //!< read next lexeme if possible
			mutable pattern_db          *dict_;  //!< dynamic dict
			
		public:
			lexemes                      cache; //!< already scanned lexemes
		};
		
	}
}

#endif
