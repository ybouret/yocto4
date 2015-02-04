#include "yocto/container/tab1d.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    
    ITableau:: ~ITableau() throw() {}
  
    ITableau:: ITableau(size_t imin, size_t imax) throw() :
    cmin( min_of(imin,imax) ),
    cmax( max_of(imin,imax) ),
    cols(cmax-cmin+1)
    {
    }
    
}

