#include "yocto/type/factory.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace hidden
    {
        void factory_reg_failure( const char *interface, const char *spec_name )
        {
            assert(interface);
            assert(spec_name);
            throw exception("factory<%s>: multiple '%s'", interface, spec_name);
        }
        
        void factory_get_failure( const char *interface, const char *spec_name )
        {
            assert(interface);
            assert(spec_name);
            throw exception("no factory<%s>('%s)", interface, spec_name);
        }

        void factory_null_address(const char *interface)
        {
            assert(interface);
            throw exception("factory<%s>() => NULL address", interface);
        }

    }
    
}

