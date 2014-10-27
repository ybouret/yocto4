#include "yocto/fame/rdomain.hpp"

namespace yocto
{
    
    namespace fame
    {
    
        RDomain::Link:: Link() throw() : position(Prev), rank(-1)
        {
        }
        
        RDomain:: Link:: ~Link() throw() {}
       
        void RDomain::Link:: set( const Position p, const int r ) throw()
        {
            (Position &)position = p;
            (int&)      rank     = r;
            assert(rank>=0);
        }
        
        
        RDomain:: Links:: Links() throw() : count(0), link()
        {
        }
        
        RDomain:: Links:: ~Links() throw()
        {
        }
        
        void RDomain:: Links:: append(const Position p, const int r) throw()
        {
            assert(count<2);
            link[count].set(p,r);
            ++( (int&)count );
        }
        
        
    }
    
}