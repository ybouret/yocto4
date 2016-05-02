#include "yocto/associative/type-glossary.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace hidden
    {
        const char type_glossary_info::name[] = "type_glossary";

        void type_glossary_info::throw_different_values(const char *the_name,const char *the_id)
        {
            assert(the_name);
            assert(the_id);
            throw imported::exception("enrolling different values for similar types","%s.enroll('%s')",the_name,the_id);
        }

    }
}

