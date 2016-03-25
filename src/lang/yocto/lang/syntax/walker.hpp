#ifndef YOCTO_LANG_SYNTAX_WALKER_INCLUDED
#define YOCTO_LANG_SYNTAX_WALKER_INCLUDED 1

#include "yocto/lang/syntax/xnode.hpp"
#include "yocto/hashing/sha1.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            class walker : public object
            {
            public:
                typedef  hashing::sha1 hash_fn;
                typedef  uint32_t      key_t;

                virtual ~walker() throw();
                mutable hash_fn hasher;

                inline key_t hash_of( const string &label ) const throw()
                {
                    return hasher.key<key_t>(label);
                }


            protected:
                explicit walker() throw();


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(walker);
            };

        }
    }
}

#endif
