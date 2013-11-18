#include "yocto/aqueous/library.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace aqueous
    {
        library:: ~library() throw() {}
        
        library:: library( size_t species_extra_bytes ) throw() :
        species::db(),
        nx(species_extra_bytes)
        {
        }
        
        species & library:: add( const string &name, int z )
        {
            species::ptr pS( new species(name,z,nx) );
            if( !insert(pS) )
                throw exception("library(multiple '%s')", name.c_str() );
            return *pS;
        }
        
        species & library:: add( const char   *id,   int z )
        {
            const string name(id);
            return add(name,z);
        }
        
        species & library:: operator[]( const string &name )
        {
            species::ptr *ppS = search(name);
            if( !ppS ) throw exception("no library['%s']", name.c_str());
            return **ppS;
        }
        
        const species & library:: operator[]( const string &name ) const
        {
            const species::ptr *ppS = search(name);
            if( !ppS ) throw exception("no library['%s']", name.c_str());
            return **ppS;
        }
        
        species       & library:: operator[]( const char *name )
        {
            const string id(name);
            return (*this)[id];
        }
        
        const species & library:: operator[]( const char *name ) const
        {
            const string id(name);
            return (*this)[id];
        }
        
                
    }
    
}
