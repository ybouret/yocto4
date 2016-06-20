#include "yocto/seem/types.hpp"

namespace yocto
{
    namespace Seem
    {
        static const char grammar_data[] =
        {
#include "seem.inc"
        };

        static inline
        lingua::parser *CreateSeemParser(const bool emitFiles)
        {
            return lingua::parser::generate(grammar_data,sizeof(grammar_data),emitFiles);
        }

        Parser:: ~Parser() throw()
        {
        }

        Parser:: Parser(const bool emitFiles) :
        impl( CreateSeemParser(emitFiles) ),
        gram( & *impl )
        {
        }
        
    }
}