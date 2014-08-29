#include "yocto/lang/pattern/dict.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        p_dict:: ~p_dict() throw()
        {
        }
        
        p_dict:: p_dict() throw() : p_dict_type()
        {
            
        }
        
        namespace
        {
            static const char fmt[] ="lang.dict(multiple '%s')";
        }
        
        void p_dict:: add(const string &name, pattern *p )
        {
            const pattern_ptr q(p);
            if( !insert(name,q) )
            {
                throw exception(fmt,name.c_str());
            }
        }
        
        void p_dict:: add(const char *name, pattern *p )
        {
            const pattern_ptr q(p);
            const string      Name(name);
            if(!insert(Name,q))
            {
                throw exception(fmt,name);
            }
        }
        
        pattern * p_dict:: operator[](const string &name) const
        {
            const pattern_ptr *pp = search(name);
            if(!pp)
                throw exception("no lang.dict['%s']", name.c_str());
            return (*pp)->clone();
        }

        pattern * p_dict:: operator[](const char *name) const
        {
            const string Name(name);
            return (*this)[Name];
        }
    }
    
}
