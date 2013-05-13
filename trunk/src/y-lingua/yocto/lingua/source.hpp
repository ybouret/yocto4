#ifndef YOCTO_LINGUA_SOURCE_INCLUDED
#define YOCTO_LINGUA_SOURCE_INCLUDED 1

#include "yocto/lingua/token.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace lingua
    {
        
        
        typedef shared_ptr<ios::istream> input;
        
        //! convert a sequence of ios::istream into a source of t_char
        class source
        {
        public:
            explicit source();
            virtual ~source() throw();
            
            t_char *get();
            void    unget(t_char *) throw();
            void    unget(token &t) throw();
            void    uncpy(const token &);
            void    skip(size_t n) throw(); //!< n <= cache.size
            
            void    attach( const input &) throw();
            void    detach() throw();
            
            size_t  cache_size() const throw();
            
        private:
            token                  cache; //!< I/O cache
            input                 *ppInp; //!< on block
            uint64_t               block[ YOCTO_U64_FOR_ITEM(input) ];
            
            void init() throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(source);
        };
    }
}


#endif
