#include "yocto/associative/lexicon.hpp"

#include <iostream>

namespace yocto
{
    namespace hidden
    {
        const char lexicon_name[] = "lexicon";
        
        void lexicon_hasher_is_invalid()
        {
            std::cerr << "You Need To Define a Specific Hasher" << std::endl;
            exit(1);
        }

    }
}
