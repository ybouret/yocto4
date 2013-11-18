#ifndef YOCTO_LINGUA_SOURCE_INCLUDED
#define YOCTO_LINGUA_SOURCE_INCLUDED 1

#include "yocto/lingua/token.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace lingua
    {
        
        
        //! shared input
        typedef shared_ptr<ios::istream> input;
        
        //! convert an input into a source of t_char
        class source
        {
        public:
            explicit source() throw();
            virtual ~source() throw();
            
            t_char *get(); //!< try to get a new t_char
            void    unget(t_char *) throw(); //!< push back into cache
            void    unget(token &t) throw(); //!< push back into cache
            void    uncpy(const token &);    //!< push a copy into cache
            void    skip(size_t n) throw();  //!< n <= cache.size
            
            void    attach( const input &) throw(); //!< detach/set as input
            void    detach() throw();               //!< remove input/cache
            bool    is_attached() const throw();
            
            size_t  cache_size() const throw();
            
            bool    is_active();          //!< if there are some available t_char
            char    peek() const throw(); //!< cache head if cache_size()>0
            
            void    attach_stdin();
            
        private:
            token                  cache; //!< I/O cache
            input                 *ppInp; //!< points on block
            uint64_t               block[ YOCTO_U64_FOR_ITEM(input) ];
            
            void init() throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(source);
        };
    }
}


#endif
