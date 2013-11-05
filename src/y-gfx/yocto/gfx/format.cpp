#include "yocto/gfx/format.hpp"

namespace yocto
{
    namespace gfx
    {
        
        
        format::format( const char *id ) :
        name(id)
        {
        }
        
        format:: ~format() throw()
        {
        }
        
        const string & format::key() const throw() { return name; }
        
        
    }
}
