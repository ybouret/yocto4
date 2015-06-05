#ifndef YOCTO_GEMS_PROPERTIES_INCLUDED
#define YOCTO_GEMS_PROPERTIES_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace gems
    {
        
        class properties
        {
        public:
            const string name; //!< given
            const word_t uuid; //!< for communication

            const string & key() const throw();
            const word_t & subkey() const throw();

            ~properties() throw();
            properties( const string &id );

            static word_t compute_uuid(const string &id) throw();



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };

    }
}

#endif
