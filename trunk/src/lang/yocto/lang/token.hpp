#ifndef YOCTO_LANG_TOKEN_INCLUDED
#define YOCTO_LANG_TOKEN_INCLUDED 1

#include "yocto/lang/types.hpp"
#include "yocto/string.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace lang
    {
        
        //! a dynamic char
        class t_char
        {
        public:
            t_char  *next;
            t_char   *prev;
            code_type code;
            
            
            static t_char *acquire(code_type value=0);
            static void    release(t_char *ch) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(t_char);
        };
        
        //! a list of dynamic char
        typedef core::list_of<t_char> t_list;

        //! a dynamic token
        class token : public object, public t_list
        {
        public:
            token() throw();                    //!< default ctor
            virtual ~token() throw();           //!< default dtor
            token( const token &other );        //!< copy content
            token & operator=( const token & ); //!< assign content
            void clear() throw();               //!< clear all
            
            token( const string & ); //!< copy string
            token( const char   * ); //!< copy string
            token( const int      ); //!< copy code
            
            token & operator=( const string & ); //!< assign string
            token & operator=( const char   * ); //!< assign string
            token & operator=( const int      ); //!< assign code
            
            string to_string(size_t nskip=0,size_t ntrim=0) const;
            void   skip(size_t n) throw();
            void   trim(size_t n) throw();
            
            friend std::ostream & operator<<( std::ostream &, const token &);
            
        };
        
        
        
    }
}

#endif
