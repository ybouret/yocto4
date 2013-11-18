#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        
        static inline
        void throw_multiple( const char *name )
        {
            assert(name);
            throw exception("%s equilibrium already defined",name);
        }
        
        void equilibria::add_water( const collection &lib, const double Kw)
        {
            equilibrium::ptr p( new constant_equilibrium("water",Kw) );
            p->add( lib["H+"], 1);
            p->add( lib["HO-"],1);
            if( !insert(p) )
                throw_multiple("water");
        }
        
        
        void equilibria:: add_acid(const collection &lib,
                                   const char       *name,
                                   const char       *acid,
                                   const char       *base,
                                   const double      Ka )
        {
            assert(name);
            assert(acid);
            assert(base);
            equilibrium::ptr p( new constant_equilibrium(name,Ka) );
            p->add(lib[acid],  -1);
            p->add(lib[base],   1);
            p->add(lib.proton(),1);
            if( !insert(p) )
                throw_multiple(name);
        }
    }
}

