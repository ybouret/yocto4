#include "yocto/spade/array-db.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace spade
    {
        array_db:: ~array_db() throw() {}
        
        array_db:: array_db() throw() : arrays()
        {
        }
        
        array_db:: array_db(size_t n) : arrays(n,as_capacity)
        {
        }
     
        void array_db:: insert( const varray::ptr &arr )
        {
            if( ! arrays.insert(arr) )
                throw exception("spade.array_db(multiple '%s')", arr->name.c_str());
        }
        
        varray & array_db:: operator[]( const string &id )
        {
            varray::ptr *ppArr = arrays.search( id );
            if( !ppArr )
                throw exception("no spade.array_db['%s']", id.c_str());
            return **ppArr;
        }
        
        
        varray & array_db:: operator[]( const char *txt )
        {
            const string id( txt );
            return (*this)[id];
        }
        
        const varray & array_db:: operator[]( const string &id ) const
        {
            const varray::ptr *ppArr = arrays.search( id );
            if( !ppArr )
                throw exception("no spade.array_db['%s']", id.c_str());
            return **ppArr;
        }
        
        
        const varray & array_db:: operator[]( const char *txt ) const
        {
            const string id( txt );
            return (*this)[id];
        }

        
    }
}
