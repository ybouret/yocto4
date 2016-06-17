#include "yocto/seem/types.hpp"

namespace yocto
{
    namespace Seem
    {
        static const char grammar_data[] =
        {
#include "seem.inc"
        };

        Parser *CreateParser(const bool emitFiles)
        {
            return lingua::parser::generate(grammar_data,sizeof(grammar_data),emitFiles);
        }

    }
}