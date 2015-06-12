#ifndef YOCTO_GEMS_PROPERTIES_INCLUDED
#define YOCTO_GEMS_PROPERTIES_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/string.hpp"
#include "yocto/ptr/nosy.hpp"
#include "yocto/associative/dual-set.hpp"
#include "yocto/counted-object.hpp"

namespace yocto
{
    namespace gems
    {

        
        class properties : public counted_object
        {
        public:
            typedef nosy_ptr<string,word_t,properties> ptr;
            typedef dual_set<string, word_t,ptr>       db;
            
            const string name; //!< given
            const word_t uuid; //!< for communication

            const string & key() const throw();
            const word_t & subkey() const throw();

            virtual ~properties() throw();
            explicit properties( const string &id );

            static word_t compute_uuid(const string &id) throw();


            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };

    }
}

#endif
