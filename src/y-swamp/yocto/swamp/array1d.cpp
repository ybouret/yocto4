#include "yocto/swamp/array1d.hpp"
#define SOURCE "array1d.cxx"

#include  <iostream>
#include "yocto/math/complex.hpp"

#include "array-inc.cxx"

namespace yocto 
{
    namespace swamp 
    {
#if 0
        template <>
        void array1D<float>:: vtk( ios::ostream &fp, const string &name, const layout_type &sub ) const
        {
            assert( this->contains(sub) );
            fp( "POINT_DATA %u\n", unsigned(sub.items) );
            fp( "SCALARS %s float\n", name.c_str());
            fp( "LOOKUP_TABLE default\n");
            for(unit_t i=sub.lower;i<=sub.upper;++i)
            {
                fp("%g\n", (*this)[i]);
            }
        }
#endif
    }
}
