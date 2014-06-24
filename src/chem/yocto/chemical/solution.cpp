#include "yocto/chemical/solution.hpp"

namespace yocto
{
    namespace chemical
    {
        
        solution:: ~solution() throw()
        {
            memory::kind<memory::global>::release_as(C,m);
            lib.decrease();
        }
        
        solution:: solution( const collection &from ) :
        lib(from),
        n( lib.size() ),
        m(n),
        C( memory::kind<memory::global>::acquire_as<double>(m) - 1 )
        {
            lib.increase();
        }
        
        size_t solution:: size() const throw()
        {
            return n;
        }

        
    }
}
