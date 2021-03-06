#include "yocto/fame/arrays.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fame
    {
        
        arrays:: ~arrays() throw()
        {}
        
        
        arrays:: arrays() throw() : arrays_type()
        {
        }
        
        arrays:: arrays(size_t n) : arrays_type(n,as_capacity)
        {
        }
        
        void arrays:: store(linear_space *l)
        {
            assert(l);
            const linear_ptr p(l);
            if( ! insert(p) )
                throw exception("multiple fields '%s'", l->name.c_str());
        }
        
        linear_space & arrays:: operator[](const string &id )
        {
            linear_ptr *pp = search(id);
            if(!pp)
                throw exception("no field '%s'", id.c_str());
            return **pp;
        }
        
        linear_space & arrays:: operator[](const char *id )
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        const linear_space & arrays:: operator[](const string &id ) const
        {
            const linear_ptr *pp = search(id);
            if(!pp)
                throw exception("no CONST field '%s'", id.c_str());
            return **pp;
        }
        
        const linear_space & arrays:: operator[](const char *id ) const
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        bool arrays:: has( const linear_space &l ) const throw()
        {
            const linear_ptr *pp = search(l.name);
            if(pp)
            {
                const linear_space &r = **pp;
                if( &r == &l )
                {
                    return true;
                }
            }
            return false;
        }
        
    }
}

