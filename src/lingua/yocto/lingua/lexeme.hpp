#ifndef YOCTO_LINGUA_LEXEME_INCLUDED
#define YOCTO_LINGUA_LEXEME_INCLUDED 1


#include "yocto/lingua/token.hpp"
#include "yocto/core/meta-list.hpp"

namespace yocto 
{
	namespace lingua
	{
		//! a token with a name (terminal) and a line
		class lexeme : public token
		{
		public:
			lexeme       *prev;   //!< for list
			lexeme       *next;   //!< for list
			const string &label;  //!< label of the rule that created the lexeme
			const size_t  line;   //!< creation line number

			explicit lexeme( const string &id, size_t il ) throw();
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
