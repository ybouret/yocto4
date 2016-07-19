#include "yocto/ptr/ref.hpp"
#include "yocto/object.hpp"

namespace yocto
{
    namespace core
    {
        size_t *create_ref() {
            size_t *ref = object::acquire1<size_t>();
            *ref = 1;
            return ref;
        }

        void    delete_ref( size_t * &ref ) throw()
        {
            assert( ref != NULL );
            assert( 0 == *ref   );
            object::release1<size_t>(ref);
            ref = NULL;
        }
        
    }
}

