#include "yocto/spade/quadmesh.hpp"
#include "yocto/error.hpp"
namespace yocto
{
    namespace spade
    {
        
        quadmesh:: ~quadmesh() throw() {}
        
        const char * quadmesh:: get_axis_name( dimension_t dim ) throw()
        {
            switch( dim )
            {
                case on_x: return "X";
                case on_y: return "Y";
                case on_z: return "Z";
            }
            // never get here
            critical_error(error_invalid_data, "quadmesh.get_axis_name(invalid dimension)");
            return "";
        }
        
        quadmesh:: quadmesh( array_db &adb_ref ) throw() : adb(adb_ref) {}
        
        
    }
}
