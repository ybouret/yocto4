#include "yocto/spade/vtk/format.hpp"

namespace yocto
{
    namespace spade
    {
        vtk_format:: vtk_format() : real_fmt( "%g" ), unit_fmt( "%ld" )
		{}
        
		vtk_format:: ~vtk_format() throw() {}
    }
    
}

