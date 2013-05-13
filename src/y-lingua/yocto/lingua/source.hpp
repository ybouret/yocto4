#ifndef YOCTO_LINGUA_SOURCE_INCLUDED
#define YOCTO_LINGUA_SOURCE_INCLUDED 1

#include "yocto/lingua/token.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/stock/stack.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto
{
    namespace lingua
    {
        
        typedef shared_ptr<ios::istream> istream_ptr;
        
        class input
        {
        public:
            const string  name;
            
            input(const string &, const istream_ptr &);
            input(const input  &);
            ~input() throw();
            
            bool query(char &C);
            
        private:
            istream_ptr iptr;
            
            YOCTO_DISABLE_ASSIGN(input);
        };
        
        //! convert a sequence of ios::istream into a source of t_char
        class source
        {
        public:
            explicit source( t_cache &p );
            virtual ~source() throw();
            
            t_char *get();
            void    unget(t_char *) throw();
            void    unget(token &t) throw();
            void    uncpy(const token &);
            
            void    push(const string &id, const istream_ptr &fp);
            
        private:
            typedef list<input>    inseq;
            t_cache               &tpool; //!< t_char pool
            token                  cache; //!< I/O cache
            stack<input,inseq>     instk; //!< stack of input
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(source);
        };
    }
}


#endif
