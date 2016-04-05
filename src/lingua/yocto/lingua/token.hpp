#ifndef YOCTO_LINGUA_TOKEN_INCLUDED
#define YOCTP_LINGUA_TOKEN_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace lingua
    {
        
        //! a dynamic char
        class t_char
        {
        public:
            t_char       *next;
            t_char       *prev;
            const uint8_t code;

            t_char(const uint8_t C) throw();
            ~t_char() throw();
            t_char(const t_char &) throw();

            typedef core::list_of_cpp<t_char> list_type;

            friend std::ostream & operator<<( std::ostream &os, const t_char &);

            YOCTO_MAKE_OBJECT
        private:
            YOCTO_DISABLE_ASSIGN(t_char);
        };

        //! a list of chars
        class token : public t_char::list_type
        {
        public:
            token() throw();
            virtual ~token() throw();
            token(const token&);
            token & operator=(const token &);

            token(const char  C);
            token(const char *s);
            token(const string &s);

            friend std::ostream & operator<<( std::ostream &os, const token &);

            string to_string(const size_t nskip=0,const size_t ntrim=0) const;

        };

    }
}

#endif
