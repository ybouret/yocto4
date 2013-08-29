#include "yocto/math/ztype.hpp"
#include "yocto/gems/factory.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/types.hpp"

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
        mass( particle_mass ),
        inv_mass( numeric<real_t>::one/mass )
        {
            
        }
        
        
        template <>
        factory<real_t>:: ~factory() throw()
        {
        }
        
        template <>
        factory<real_t>:: factory() :
        records(),
        typeIDs()
        {
        }
        
        template <>
        record<real_t> & factory<real_t>:: declare( const string &name, const uint_t type, const real_t mass)
        {
            if( typeIDs.search(type) )
                throw exception("gems::factory(multiple id 0x%08lx)", long(type) );
            
            record<real_t>::ptr p( new record<real_t>(name,type,mass) );
            try
            {
                if( ! records.insert(p) )
                    throw exception("gems::factory(multiple '%s')", name.c_str());
                
                if( ! typeIDs.insert(type,name) )
                    throw exception("gems::factory(unexpected id failure for '%s')", name.c_str());
            }
            catch(...)
            {
                (void) records.remove(name);
                throw;
            }
            return *p;
        }
        
        template<>
        record<real_t> & factory<real_t>:: operator[](const string &name)
        {
            record<real_t>::ptr *ppRec = records.search(name);
            if(!ppRec) throw exception("no gems::factor[%s]", name.c_str());
            record<real_t> &rec = **ppRec;
            assert( typeIDs.search(rec.type) != 0);
            return rec;
        }
        
        template<>
        record<real_t> & factory<real_t>:: operator[](const char *name)
        {
            const string NAME(name);
            return (*this)[NAME];
        }
        
        template<>
        const record<real_t> & factory<real_t>:: operator[](const string &name) const
        {
            const record<real_t>::ptr *ppRec = records.search(name);
            if(!ppRec) throw exception("no gems::factor[%s] const", name.c_str());
            const record<real_t> &rec = **ppRec;
            assert( typeIDs.search(rec.type) != 0);
            return rec;
        }
        
        template<>
        const record<real_t> & factory<real_t>:: operator[](const char *name) const
        {
            const string NAME(name);
            return (*this)[NAME];
        }
        
        
    }
    
}