#ifndef YOCTO_LANG_LEXEME_INCLUDED
#define YOCTO_LANG_LEXEME_INCLUDED 1


#include "yocto/lang/token.hpp"
#include "yocto/core/meta-list.hpp"

namespace yocto
{
	namespace lang
	{
		//! a token with a name (terminal) and a line
		class lexeme : public token
		{
		public:
			lexeme       *prev;   //!< for list
			lexeme       *next;   //!< for list
			const string &label;  //!< label of the rule that created the lexeme
			const int     line;   //!< creation line number
            
			explicit lexeme( const string &id, int il ) throw();
			virtual ~lexeme() throw();
			lexeme( const lexeme &other );
            
			lexeme *clone() const;
            
		private:
			YOCTO_DISABLE_ASSIGN(lexeme);
		};
        
		typedef core::meta_list<lexeme> lexemes;
        
	}
}

#endif
