#include "yocto/gems/type-table.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace gems
    {
        type_table:: ~type_table() throw()
        {
            
        }
        
        type_table:: type_table() throw() :
        dmap(), rmap()
        {
            
        }
        
        namespace
        {
            static const char fn[] = "type_table";
        }
        
        void type_table:: operator()(word_t t, const string &s)
        {
            if( dmap.search(t) ) throw exception("%s: type is already registered",fn);
            if( rmap.search(s) ) throw exception("%s: '%s' is already registered",fn,s.c_str());
            
            if( !dmap.insert(t, s) )
                throw exception("%s:unexpected direct map insertion failure",fn);
            try
            {
                if( !rmap.insert(s,t) )
                    throw exception("%s:unexpected reverse map insertion failure",fn);
            }
            catch(...)
            {
                (void) dmap.remove(t);
                throw;
            }
        }
        
        void type_table:: operator()( word_t t, const char   *s)
        {
            const string id(s);
            (*this)(t,id);
        }

        word_t type_table:: operator[]( const string &s ) const
        {
            const word_t *pw = rmap.search(s);
            if(!pw) throw exception("%s:unregistered '%s'", fn, s.c_str());
            return *pw;
        }

        word_t type_table:: operator[]( const char *s ) const
        {
            const string id(s);
            return (*this)[id];
        }
        
        const string & type_table:: operator[]( word_t t ) const
        {
            const string *ps = dmap.search(t);
            if(!ps)
                throw exception("%s: unregistered type", fn);
            return *ps;
        }
        
    }
}
