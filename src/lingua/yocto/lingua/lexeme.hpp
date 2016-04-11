#ifndef YOCTO_LINGUA_LEXEME_INCLUDED
#define YOCTO_LINGUA_LEXEME_INCLUDED 1

#include "yocto/lingua/token.hpp"

namespace yocto
{
    namespace lingua
    {

        class lexeme : public token
        {
        public:
            lexeme       *next;   //!< for list
            lexeme       *prev;   //!< for list
            const string &label;  //!< lexical::rule label
            const int     line;   //!< line creation

            //! setup and steal token content
            explicit lexeme(const string &the_label,
                            const int     the_line,
                            token        &the_token) throw();
            
            virtual ~lexeme() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexeme);
        };

        typedef core::list_of_cpp<lexeme> l_list;

    }
}

#endif

