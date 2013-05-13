#ifndef YOCTO_LINGUA_TOKEN_INCLUDED
#define YOCTO_LINGUA_TOKEN_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
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
        
        //! a cache of t_char
        class t_cache : public core::pool_of<t_char>
        {
        public:
            explicit t_cache() throw();
            virtual ~t_cache() throw();
            void reserve(size_t n);
            
             t_char  * create(char C=0);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(t_cache);
        };
        
        
        //! a dynamic list of t_char
        class token : public core::list_of<t_char>
        {
        public:
            explicit token(t_cache &p) throw();
            virtual ~token() throw();
            token( const token &other);
            
            void clear() throw();

            string to_string( size_t skip=0,size_t trim=0 ) const;
            
            friend std::ostream & operator<<( std::ostream &, const token &);
            
        private:
            YOCTO_DISABLE_ASSIGN(token);
            t_cache &cache;
        };
    
    }
}

#endif
