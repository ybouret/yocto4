#ifndef YOCTO_LANG_TOKEN_INCLUDED
#define YOCTO_LANG_TOKEN_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    
    namespace lang
    {
        
        typedef int code_t;
        
        class t_char
        {
        public:
            t_char *next;
            t_char *prev;
            code_t  code;

            YOCTO_MAKE_OBJECT
            t_char() throw();
            t_char(code_t C) throw();
            ~t_char() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(t_char);
        };
        
        typedef core::list_of_cpp<t_char> t_list;
        
        
        //! a basic token
        /**
         with already 'clear' and 'swap_with' methods.
         */
        class token : public t_list
        {
        public:
            explicit token() throw();
            token(const token &other);
            token & operator=( const token &other);
            virtual ~token() throw();
            
            friend std::ostream & operator<<( std::ostream &os, const token &);
            
            string to_string() const;
        };
        
    }
}


#endif

