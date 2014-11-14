#include "yocto/spade/variables.hpp"

namespace yocto
{
    namespace spade
    {
     
        variables:: ~variables() throw() {}
        
        variables:: variables() throw() : variables_base() {}
        
        variables:: variables( const variables &other ) : variables_base(other) {}
        
        void variables:: append( const string &id )
        {
            push_back(id);
        }

        
        void variables:: append( const char *id )
        {
            const string ID(id);
            push_back(ID);
        }
        
        variables & variables:: operator<<( const string &id )
        {
            append(id);
            return *this;
        }
        
        variables & variables:: operator<<( const char *id )
        {
            append(id);
            return *this;
        }
        
    }
}
