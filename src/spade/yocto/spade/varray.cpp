#include "yocto/spade/varray.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace spade
    {
        
        varray::~varray() throw()
        {
            assert(info);
            assert(addr);
            assert(dtor);
            dtor(addr);
            addr = 0;
            info = 0;
            dtor = 0;
        }
        
        varray::varray(const string         &array_name,
                       const type_spec      &array_spec,
                       const type_spec      &array_held,
                       void *                array_addr,
                       linear               *array_info,
                       void                (*array_dtor)(void *)) :
        name( array_name ),
        spec( array_spec ),
	held( array_held ),
        addr( array_addr ),
        info( array_info ),
        dtor( array_dtor ),
        data( *info )
        {
        }
        
        const string & varray:: key() const throw() { return name; }
        
        void varray:: check_spec( const type_spec &requested ) const
        {
            if( requested != spec )
                throw exception("spade.varray.spec=%s != %s", spec.name(), requested.name() );
        }
        
        
        linear & varray:: handle() throw()
        {
            assert(info);
            return *info;
        }
        
        const linear & varray:: handle() const throw()
        {
            assert(info);
            return *info;
        }
    }
}
