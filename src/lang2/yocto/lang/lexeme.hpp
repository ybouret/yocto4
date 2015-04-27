#ifndef YOCTO_LANG_LEXEME_INCLUDED
#define YOCTO_LANG_LEXEME_INCLUDED 1

#include "yocto/lang/token.hpp"

namespace yocto
{
    namespace lang
    {

        //! a lexeme is a token produced by a rule
        class lexeme : public token
        {
        public:
            lexeme *       next;  //!< for list
            lexeme *       prev;  //!< for list
            const string & label; //!< reference to the producing rule
            const int      line;  //!< creation line

            virtual ~lexeme() throw();
            explicit lexeme(const string &l, const int i) throw();
            lexeme(const lexeme &other);

        private:
            YOCTO_DISABLE_ASSIGN(lexeme);
        };

        //! lexeme list
        typedef core::list_of_cpp<lexeme> l_list;
    }
}

#endif