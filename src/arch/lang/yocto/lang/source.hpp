#ifndef YOCTO_LANG_TCHAR_SOURCE_INCLUDED
#define YOCTO_LANG_TCHAR_SOURCE_INCLUDED 1

#include "yocto/lang/token.hpp"
#include "yocto/ios/istream.hpp"

namespace yocto
{
    namespace lang
    {

        //! converts an input stream
        /**
         the base class token is used as a cache.
         */
        class source : public token
        {
        public:
            explicit source() throw();
            virtual ~source() throw();

            //! return a new t_char
            /**
             from this(=cache) if not empty, 
             then from fp, NULL otherwise.
             */
            t_char       *get( ios::istream &fp );

            //! try to return an old or new head char
            const t_char *peek( ios::istream &fp );

            //! skip n<=size chars
            void skip(size_t n) throw();

            //! unget a t_char
            void unget( t_char *ch  ) throw();

            //! unget a token
            void unget( token  &tkn ) throw();

            //! unget a copy a token
            void uncpy( const token &tkn );

            //! unget a string
            void unget( const string &s );

            //! unget a char
            void unget( const char C );

            //! unget a C string
            void unget( const char *s );

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(source);

        };
    }
}

#endif

