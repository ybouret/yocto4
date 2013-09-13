#include "yocto/gems/properties.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gems
    {
        
        
        properties:: ~properties() throw() 
        {
        }
     
        properties:: properties( word_t t, const string &n ) :
        type(t),
        name(n)
        {
        }
        
        
        const word_t & properties:: key()    const throw() { return type; }
        const string & properties:: subkey() const throw() { return name; }
     
        void properties:: check_pointer(const void *addr, const string &id)
        {
            if(!addr) throw exception("unregistered name='%s'", id.c_str());
        }
        
        void properties:: check_pointer(const void *addr, word_t id)
        {
            if(!addr) throw exception("unregistered type=0x%08x", unsigned(id) );
        }
        
    }
    
}
