#include "yocto/container/tab1d.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    
    ITableau:: ~ITableau() throw() {}
  
    ITableau:: ITableau(size_t __imin, size_t __imax) throw() :
    imin( min_of(__imin,__imax) ),
    imax( max_of(__imin,__imax) ),
    cols(imax-imin+1)
    {
    }
    
    
    
}