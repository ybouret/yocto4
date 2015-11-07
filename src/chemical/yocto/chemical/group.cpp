#include "yocto/chemical/group.hpp"


namespace yocto
{
    
    namespace chemical
    {
        const string & group::key() const throw() { return label; };
        
        
        
        group::group(const string &grpLabel,
                     const int     grpCount) :
        label(grpLabel),
        count(grpCount)
        {
        }
        
        group:: ~group() throw()
        {
        }
        

        
    }
    
}