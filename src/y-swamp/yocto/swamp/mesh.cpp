#include "yocto/swamp/mesh.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    namespace swamp 
    {
        
        quadmesh:: ~quadmesh() throw() {}
        quadmesh::  quadmesh( array_db &db ) throw() :adb( db ) {}
        
        const char *quadmesh:: axis_name( size_t iAxis )
        {
            switch( iAxis )
            {
                case 0:
                    return "X";
                
                case 1:
                    return "Y";
                    
                case 2:
                    return "Z";
                    
                default:
                    break;
            }
            throw exception("invalid quadmesh::axis_name(%u)", unsigned(iAxis) );
        }

        const char *quadmesh:: delta_name( size_t iAxis )
        {
            switch( iAxis )
            {
                case 0:
                    return "dX";
                    
                case 1:
                    return "dY";
                    
                case 2:
                    return "dZ";
                    
                default:
                    break;
            }
            throw exception("invalid quadmesh::delta_name(%u)", unsigned(iAxis) );
        }

        
    }
    
}
