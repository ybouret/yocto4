#ifndef YOCTO_LINGUA_TOKEN_INCLUDED
#define YOCTO_LINGUA_TOKEN_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace lingua
    {
        
        //! a single dynamic char
        class t_char
        {
        public:
            t_char *next; //!< for list
			t_char *prev; //!< for list
			char    data; //!< actual char
            
            static t_char *acquire( char C = 0 );
            static void    release(t_char *) throw();
            
            
        private:
            t_char(); ~t_char();
            YOCTO_DISABLE_COPY_AND_ASSIGN(t_char);
        };
        
        
        //! a dynamic list of t_char
        class token : public core::list_of<t_char>
        {
        public:
            explicit token() throw();    //!< empty token
            virtual ~token() throw();              //!< back into cache
            token(const token &other);             //!< copy with same cache
            
            explicit token(const string &s);
            explicit token( const char   *s);
            
            token & operator=( const token  &);
            token & operator=( const string &);
            token & operator=( const char *);
            
            void clear() throw();
            
            string to_string( size_t nskip=0,size_t ntrim=0 ) const;
            
            friend std::ostream & operator<<( std::ostream &, const token &);
            
            void skip() throw(); //!< if size > 0
            void trim() throw(); //!< if size > 0
            
        };
        
    }
}

#endif
