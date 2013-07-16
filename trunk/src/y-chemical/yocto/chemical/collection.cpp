#include "yocto/chemical/collection.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        collection:: ~collection() throw() {}
        
        collection:: collection() throw() {}
        
        collection:: collection(size_t n) :
        species::db(n,as_capacity)
        {
            
        }
        
        
        species & collection::add( const string &name, int z)
        {
            species::ptr p( new species(name,z) );
            if( !insert(p) )
                throw exception("chemical.collection.add(mutliple '%s')", name.c_str());
            return *p;
        }

        
        species::ptr & collection:: operator[]( const string &id )
        {
            species::ptr *pp = search(id);
            if( !pp )
                throw exception("chemical.collection[no '%s']", id.c_str());
            return *pp;
        }
        
        species::ptr & collection:: operator[]( const char *id )
        {
            const string ID(id);
            return (*this)[ID];
        }

        const species::ptr & collection:: operator[]( const string &id ) const
        {
            const species::ptr * pp = search(id);
            if( !pp )
                throw exception("chemical.collection[no '%s'].const", id.c_str());
            return *pp;
        }
        
        const species::ptr & collection:: operator[]( const char *id ) const 
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        
        

        
        
    }

}
