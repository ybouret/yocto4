#ifndef YOCTO_RX_LEXER_INCLUDED
#define YOCTO_RX_LEXER_INCLUDED 1

#include "yocto/rx/lexical/atom.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/rx/source.hpp"

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
			
			void operator()( pattern *motif, const string &label, const lexical::action *a = NULL);
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
						
			inline pattern_db       &dict()       { if( !dict_ ) dict_ = new pattern_db(); return *dict_; }
			inline const pattern_db &dict() const { if( !dict_ ) dict_ = new pattern_db(); return *dict_; }
			inline void              no_dict() throw() { if( dict_ ) { delete dict_; dict_=NULL; } }
			
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
