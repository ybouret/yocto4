#ifndef YOCTO_LANG_SOURCE_INCLUDED
#define YOCTO_LANG_SOURCE_INCLUDED 1


#include "yocto/lang/token.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
    namespace lang
    {
        
        //! convert an istream into a t_char stream
        class source
        {
        public:
            explicit source() throw();
            virtual ~source() throw();
            
            t_char *get(ios::istream &);      //!< return next char, NULL=>EOF
            void    unget(t_char *) throw();  //!< unread the char
            void    unget(token  &) throw();  //!< unread the token
            void    uncpy(const token&);      //!< unread a copy
            
            void reset() throw(); //!< empty cache
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(source);
            token cache;
        };
    
    }
}

#endif

