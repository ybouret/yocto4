#ifndef YOCTO_GEMS_PROPERTIES_INCLUDED
#define YOCTO_GEMS_PROPERTIES_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/associative/dual-set.hpp"
#include "yocto/string.hpp"
#include "yocto/ptr/nosy.hpp"

namespace yocto
{
    namespace gems
    {

        class properties : public counted_object
        {
        public:

            const string name; //!< user's name
            const word_t uuid; //!< uuid, for transfert of atoms
            const double mass; //!< double precision mass
            vslot        data; //!< for force field etc..
            virtual ~properties() throw();
            explicit properties(const string &id, const double m);
            const string &key() const throw();
            const word_t &subkey() const throw();

            static word_t compute_uuid(const string &) throw();

            typedef nosy_ptr<string,word_t,properties> pointer;
            typedef dual_set<string,word_t,pointer>    database;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };

        
        class library
        {
        public:
            explicit library();
            virtual ~library() throw();

        private:
            properties::database db;
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
    }

}


#endif
