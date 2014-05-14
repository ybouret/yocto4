#include "yocto/container/tab1d.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    ITableau:: ~ITableau() throw() {}
    
    void ITableau::Check(size_t __imin, size_t __imax)
    {
        
        if(__imax<__imin)
            throw exception("Invalid Tableau Indices");
    }

    ITableau:: ITableau(size_t __imin, size_t __imax) :
    imin(__imin),
    imax(__imax)
    {
       Check(imin,imax);
    }
    
    
    size_t ITableau::SizeOf(size_t __imin, size_t __imax)
    {
        Check(__imin, __imax);
        return __imax+1-__imin;
    }
    
    
}