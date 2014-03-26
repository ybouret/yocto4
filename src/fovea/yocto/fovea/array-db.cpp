#include "yocto/fovea/array-db.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        
        array_db:: ~array_db() throw()
        {}
        
        
        array_db:: array_db() throw() : adb_type()
        {
        }
        
        void array_db:: store(linear_space *l)
        {
            assert(l);
            const linear_ptr p(l);
            if( ! insert(p) )
                throw exception("multiple fields '%s'", l->name.c_str());
        }
        
        linear_space & array_db:: operator[](const string &id )
        {
            linear_ptr *pp = search(id);
            if(!pp)
                throw exception("no field '%s'", id.c_str());
            return **pp;
        }
        
        linear_space & array_db:: operator[](const char *id )
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        const linear_space & array_db:: operator[](const string &id ) const
        {
            const linear_ptr *pp = search(id);
            if(!pp)
                throw exception("no CONST field '%s'", id.c_str());
            return **pp;
        }
        
        const linear_space & array_db:: operator[](const char *id ) const
        {
            const string ID(id);
            return (*this)[ID];
        }

        
    }
}