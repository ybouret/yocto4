#include "yocto/swamp/arrays.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace swamp 
    {
        array_db:: array_db() throw() : arrays()
        {
        }
        
        array_db:: ~array_db() throw()
        {
        }
        
        void array_db:: record(const string         &name, 
                               const std::type_info &which, 
                               void                 *addr,
                               linear_base          *info,
                               void                (*kill)( void *)
                               )
        {
            assert( addr != NULL );
            assert( kill != NULL );
            
            //------------------------------------------------------------------
            // create a raw array
            //------------------------------------------------------------------
            array *arr = NULL;
            try 
            {
                arr = new array(name,which,addr,info,kill);
            }
            catch(...)
            {
                kill( addr );
                throw;
            }
            
            //------------------------------------------------------------------
            // take care of it
            //------------------------------------------------------------------
            const array::ptr p( arr );
            
            //------------------------------------------------------------------
            // record it
            //------------------------------------------------------------------
            if( ! arrays.insert(p) )
            {
                throw exception("swamp::arrays(multiple '%s')", name.c_str() );
            }
        }
        
        array & array_db:: operator[]( const string &name ) 
        {
            array::ptr *pArr = arrays.search( name );
            if( !pArr )
                throw exception("no array '%s'", name.c_str() );
            return **pArr;
        }
        
    }
}