#include "yocto/chemical/initializer.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace chemical
    {
        
        constraint:: constraint( const double v ) :
        constraint_weights(),
        value(v)
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
        
        void initializer:: electroneutrality( const collection &lib )
        {
            constraint &Q = create(0);
            for( collection::const_iterator i=lib.begin(); i!=lib.end();++i)
            {
                const species &sp = **i;
                Q[sp.name] = sp.z;
            }
        }
        
        
        std::ostream & operator<<(std::ostream &os, const constraint &Q)
        {
            os << Q.value << " = ";
            size_t j=0;
            for( constraint_weights::const_iterator i = Q.begin(); i != Q.end(); ++i, ++j )
            {
                const string &name = i->key;
                const double  w    = *i;
                if(j>0) os << " + ";
                os << "(" << w << ")*[" << name << "]";
            }
            return os;
        }
        
        std::ostream & operator<<( std::ostream &os, const initializer &I )
        {
            static const size_t ns = 64;
            for(size_t i=0;i<ns;++i) os << ':'; os << std::endl;
            for(size_t i=1; i <= I.size(); ++i)
            {
                os << ": " << *I[i] << std::endl;
            }
            for(size_t i=0;i<ns;++i) os << ':'; os << std::endl;
            return os;
        }
        
    }
}
