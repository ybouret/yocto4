#include "yocto/chemical/solution.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    
    namespace chemical
    {
        
        bool solution:: has_same_components_than(const solution &other ) const throw()
        {
            if(other.components != components)
            {
                return false;
            }
            
            const_iterator i=composition.begin();
            const_iterator j=other.composition.begin();
            
            for(size_t n = components;n>0;--n,++i,++j)
            {
                const component &mine = *i;
                const component &its  = *j;
                if( mine.spec->name != its.spec->name )
                    return false;
            }
            return true;
            
        }
        
        void solution:: add( const solution &other )
        {
            solution &self = *this;
            for( const_iterator i = other.begin(); i != other.end(); ++i )
            {
                const component &comp = *i;
                self[ comp.spec->name ] += comp.concentration;
            }
        }

        void solution:: mul(double a) throw()
        {
            for( component::db::iterator i=composition.begin(); i != composition.end(); ++i )
            {
                component &comp = *i;
                comp.concentration *= a;
            }
        }

    }
    
}

