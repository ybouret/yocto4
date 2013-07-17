#include "yocto/chemical/solution.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace chemical
    {
        
        component:: ~component() throw() {}
        
        component:: component( const species::ptr &sp ) throw() :
        spec(sp),
        conc(0)
        {
        }
        
        const string & component:: key() const throw()
        {
            return spec->name;
        }
        
        component:: component( const component &other ) throw() :
        spec(other.spec),
        conc(other.conc)
        {
            
        }
        
        solution:: solution( const collection &lib ) :
        components( lib.size() ),
        composition( components, as_capacity)
        {
            for(collection::const_iterator i=lib.begin(); i != lib.end(); ++i )
            {
                const species::ptr &spec = *i;
                const component     comp(spec);
                if( !composition.insert(comp) )
                    throw exception("solution(unexpected failure for '%s')", spec->name.c_str());
            }
        }
        
        solution:: ~solution() throw()
        {
        }
        
        double & solution:: operator[]( const string &name )
        {
            component *p = composition.search(name);
            if(!p)
                throw exception("solution[no '%s']", name.c_str());
            return p->conc;
        }
        
        const double & solution:: operator[]( const string &name ) const
        {
            const component *p = composition.search(name);
            if(!p)
                throw exception("solution[no '%s'] const", name.c_str());
            return p->conc;
        }
        
        double & solution:: operator[]( const char *id )
        {
            const string name(id);
            return (*this)[name];
        }
        
        const double & solution:: operator[]( const char *id ) const
        {
            const string name(id);
            return (*this)[name];
        }
        
        double solution::pH() const
        {
            return -math::Log10( (*this)["H+"] );
        }
        
    }
    
}

