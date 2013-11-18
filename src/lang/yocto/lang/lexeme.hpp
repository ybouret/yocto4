#ifndef YOCTO_LANG_LEXEME_INCLUDED
#define YOCTO_LANG_LEXEME_INCLUDED 1

#include "yocto/rx/token.hpp"
#include "yocto/core/meta-list.hpp"

namespace yocto 
{
    
    namespace lang
    {
        
        //! generic lexeme
        class lexeme : public regex::token
        {
        public:
            const string &label; //!< reference to the terminal token
            const size_t  line;  //!< line of existence
            lexeme       *next;  //!< for linked list 
            lexeme       *prev;  //!< for linked list
            
            explicit lexeme( const string &n, size_t l ) throw();
            virtual ~lexeme() throw();
            
            lexeme * clone() const;
            
        private:
            lexeme(const lexeme&); //!< copy token as well
            lexeme&operator=(const lexeme&);
        };
        
        
        //! list of lexemes
        typedef core::meta_list<lexeme> lexemes;
        
    }
    
}

#endif
