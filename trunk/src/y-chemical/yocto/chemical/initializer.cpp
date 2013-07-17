#include "yocto/chemical/initializer.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    namespace chemical
    {
        
        constraint:: constraint( const double v ) :
        constraint_weights(),
        value()
        {
        }
        
        constraint:: ~constraint() throw()
        {
        }
        
        double & constraint:: operator[]( const string &name )
        {
            double *p = search(name);
            if(!p)
            {
                if( !insert(name, 0) )
                    throw exception("unexpected constraint['%s'] insert failure", name.c_str());
                p = search(name);
                if(!p)
                    throw exception("unexpected constraint['%s'] search failure", name.c_str());
                
            }
            assert(p);
            return *p;
        }
        
        double & constraint:: operator[]( const char *id )
        {
            const string name(id);
            return (*this)[name];
        }
        
        const double & constraint:: operator[]( const string &name ) const
        {
            const double * p = search(name);
            if(!p) throw exception("constraint[no '%s']", name.c_str());
            return *p;
        }
        
        
        const double & constraint:: operator[]( const char *id ) const
        {
            const string name(id);
            return (*this)[name];
        }
        
        
        initializer:: ~initializer() throw()
        {
        }
        
        initializer:: initializer() throw() : constraints()
        {
        }
        
        
        constraint & initializer:: create( const double value )
        {
            constraint::ptr p( new constraint(value) );
            push_back(p);
            return *p;
        }
        
        
        
    }
}
