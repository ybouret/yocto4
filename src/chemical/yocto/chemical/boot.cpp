#include "yocto/chemical/boot.hpp"

namespace yocto
{
    namespace chemical
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        boot:: constituent:: constituent(const species::ptr &sp,
                                         const double        w ) throw() :
        spec(sp),
        weight(w)
        {
            assert(fabs(w)>0);
        }
        
        boot:: constituent:: constituent( const constituent &other ) throw() :
        spec( other.spec ),
        weight( other.weight )
        {
        }
        
        boot:: constituent:: ~constituent() throw() {}
        
        const string & boot:: constituent:: key() const throw()
        {
            return spec->name;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        boot:: constraint:: constraint( const double v ) throw() :
        constituents(),
        value(v)
        {
        }
        
        boot:: constraint:: ~constraint() throw() {}
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        boot:: loader:: loader() throw() : constraints() {}
        boot:: loader:: ~loader() throw() {}
        
        
        boot::constraint & boot:: loader:: add(double value)
        {
            const constraint::ptr p( new constraint(value) );
            push_back(p);
            return *back();
        }
        
        
    }
    
}

