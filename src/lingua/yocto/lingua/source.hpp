#ifndef YOCTO_LINGUA_SOURCE_INCLUDED
#define YOCTO_LINGUA_SOURCE_INCLUDED 1

#include "yocto/lingua/token.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/sequence/addr-list.hpp"
namespace yocto
{
    namespace lingua
    {
        class source
        {
        public:
            explicit source() throw();
            explicit source(ios::istream&) throw();
            virtual ~source() throw();

            //! read a t_char from input(s)
            t_char *query();

            //! store a previously read t_char
            void    store(t_char *ch) throw();

            //! store a previously read token
            void    unread(token  &tkn) throw();

            //! unread a copy
            void    unread_a_copy_of(const token &tkn);
            
            bool    is_active();

            //! attached a new input, save status
            void attach( ios::istream & );

            //! return next t_char if possible
            const t_char *peek();

            //! number of read chars in cache
            const size_t read() const throw();

            //! skip n <= read() chars
            void         skip(size_t n) throw();

        private:
            typedef addr_node<ios::istream> input_node;
            typedef addr_list<ios::istream> input_list;

            ios::istream *input;
            token         cache;
            input_list    attached;
            t_char       *query_from_input();
            YOCTO_DISABLE_COPY_AND_ASSIGN(source);
        };
    }
}

#endif
