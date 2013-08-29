#include "yocto/math/ztype.hpp"
#include "yocto/gems/inventory.hpp"

namespace yocto
{
    namespace gems
    {
        using namespace math;
        
        template <>
        record<real_t>:: ~record() throw() {}
        
        template <>
        const string &record<real_t>:: key() const throw()
        {
            return name;
        }
        
        template <>
        record<real_t>:: record(const string &particle_name,
                                const uint_t  particle_type,
                                real_t        particle_mass ) :
        name( particle_name ),
        type( particle_type ),
        mass( particle_mass )
        {
            
        }
        
        
        template <>
        inventory<real_t>:: ~inventory() throw()
        {
        }
        
        template <>
        inventory<real_t>:: inventory()
        {
        }
        
        template <>
        record<real_t> & inventory<real_t>:: declare( const string &name, const uint_t type, const real_t mass)
        {
            const record<real_t>::ptr p( new record<real_t>(name,type,mass) );
        }

        
    }
    
}