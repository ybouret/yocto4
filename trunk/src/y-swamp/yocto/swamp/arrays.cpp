#include "yocto/swamp/arrays.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace swamp 
    {
        
        varray:: varray(const string         &array_name, 
                        const type_spec      &array_spec,
                        void *                array_addr,
                        linear_base          *array_info,
                        void                (*array_kill)(void *)) :
        name(  array_name ),
        spec(  array_spec ),
        addr(  array_addr ),
        kill(  array_kill ),
        data( *array_info )
        {
            assert( addr != NULL );
            assert( kill != NULL );
        }
        
        
        varray:: ~varray() throw()
        {
            kill( addr );
        }

        const string & varray:: key() const throw() { return name; }
        
        
        array_db:: array_db() throw() : arrays()
        {
        }
        
        array_db:: ~array_db() throw()
        {
        }
        
        void array_db:: append(const string         &name, 
                               const type_spec      &spec, 
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
            varray *arr = NULL;
            try 
            {
                arr = new varray(name,spec,addr,info,kill);
            }
            catch(...)
            {
                kill( addr );
                throw;
            }
            
            //------------------------------------------------------------------
            // take care of it
            //------------------------------------------------------------------
            const varray::ptr p( arr );
            
            //------------------------------------------------------------------
            // record it
            //------------------------------------------------------------------
            if( ! arrays.insert(p) )
            {
                throw exception("swamp::arrays(multiple '%s')", name.c_str() );
            }
        }
        
        varray & array_db:: operator[]( const string &name ) 
        {
            varray::ptr *pArr = arrays.search( name );
            if( !pArr )
                throw exception("no array '%s'", name.c_str() );
            return **pArr;
        }
        
        const varray & array_db:: operator[]( const string &name ) const
        {
            varray::ptr const *pArr = arrays.search( name );
            if( !pArr )
                throw exception("no const array '%s'", name.c_str() );
            return **pArr;
        }
        
        
    }
}