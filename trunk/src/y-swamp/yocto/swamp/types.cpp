#include "yocto/swamp/types.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        offsets_list:: ~offsets_list() throw()
        {
        }
        
        offsets_list:: offsets_list() throw() : sorted_offsets() {}
        
        offsets_list:: offsets_list( size_t num ) : sorted_offsets( num, as_capacity)
        {
        }
        
        offsets_list:: offsets_list( const offsets_list &other ) : sorted_offsets( other )
        {
        }
        
        void offsets_list:: store( size_t indx )
        {
            (void) insert(indx);
        }
        
        
        variables:: ~variables() throw()
        {
        }
        
        variables:: variables() throw() : vector<string>()
        {
        }
        
        variables:: variables( size_t n ) : vector<string>(n,as_capacity)
        {
        }
        
        variables:: variables( const variables &other ) : vector<string>( other )
        {
        }
        
        variables & variables:: operator<<( const string &name ) 
        {
            push_back( name );
            return *this;
        }
        variables & variables:: operator<<( const char *id ) 
        {
            const string name(id);
            push_back(name);
            return *this;
        }
    }
    
}
