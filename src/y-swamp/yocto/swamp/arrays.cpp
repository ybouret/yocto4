#include "yocto/swamp/arrays.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace swamp 
    {
        
        static inline void *varray_check_addr( void *array_addr )
        {
            if( !array_addr )
                throw exception("varray: NULL address");
            return array_addr;
        }
        
        static inline linear_base *varray_check_info( linear_base *array_info )
        {
            if( !array_info )
                throw exception("varray: NULL linear_base");
            return array_info;
        }
        
        varray:: varray(const string         &array_name, 
                        const type_spec      &array_spec,
                        const type_spec      &array_held,
                        void *                array_addr,
                        linear_base          *array_info,
                        void                (*array_kill)(void *)) :
        name(  array_name ),
        spec(  array_spec ),
        held(  array_held ),
        addr(  varray_check_addr(array_addr) ),
        kill(  array_kill ),
        info( varray_check_info(array_info) ),
        data( *info )
        {
            if(!kill)
                throw exception("varray: NULL destructor");
        }
        
        linear_base * varray:: handle() throw()
        {
            return info;
        }
        
        const linear_base * varray:: handle() const throw()
        {
            return info;
        }

        varray:: ~varray() throw()
        {
            kill( addr );
        }
        
        const string & varray:: key() const throw() { return name; }
        
        void varray:: check_specs( const type_spec &self, const type_spec &required)
        {
            if( self != required )
                throw exception("varray: '%s' is not '%s'", self.name(), required.name());
        }
        
        
        array_db:: array_db() throw() : arrays() {}
        array_db:: array_db( size_t n) : arrays(n,as_capacity) {}
        
        array_db:: ~array_db() throw() {}
        
        
        void array_db:: operator()(const string         &name, 
                                   const type_spec      &spec, 
                                   const type_spec      &held,
                                   void                 *addr,
                                   linear_base          *info,
                                   void                (*kill)( void *)
                                   )
        {
            assert( addr != NULL );
            assert( kill != NULL );
            
            //------------------------------------------------------------------
            // create a raw varray
            //------------------------------------------------------------------
            varray *arr = NULL;
            try 
            {
                //--------------------------------------------------------------
                // create the array with its memory
                //--------------------------------------------------------------
                arr = new varray(name,spec,held,addr,info,kill);
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
        
        varray       & array_db:: operator[]( const char *id )
        {
            const string name(id);
            return (*this)[ name ];
        }
        
        
        const varray & array_db:: operator[]( const char *id ) const
        {
            const string name(id);
            return (*this)[ name ];
        }
        
    }
}