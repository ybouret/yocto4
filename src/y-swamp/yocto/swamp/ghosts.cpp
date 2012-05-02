#include "yocto/swamp/ghosts.hpp"
#include "yocto/error.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        ghost:: ~ghost() throw() {}
        
        ghost:: ghost( ghost::position p ) throw() :
        site( p ),
        offsets()
        {
        }
        
        const char * ghost:: get_position_name( ghost::position p ) throw()
        {
            switch( p )
            {
                case at_lower_x: return "lower x";
                case at_upper_x: return "upper x";
                case at_lower_y: return "lower y";
                case at_upper_y: return "upper y";
                case at_lower_z: return "lower z";
                case at_upper_z: return "upper z";
            }
            critical_error(error_invalid_data, "invalid ghost::position name");
        }
        
        const char *ghost:: position_name() const throw()
        {
            return get_position_name(site);
        }
        
        ghost::position ghost:: get_mirror_position( ghost::position p ) throw()
        {
            switch( p )
            {
                case at_lower_x: return at_upper_x;
                case at_upper_x: return at_lower_x;
                case at_lower_y: return at_upper_y;
                case at_upper_y: return at_lower_y;
                case at_lower_z: return at_upper_x;
                case at_upper_z: return at_lower_z;
            }
            critical_error(error_invalid_data, "invalid ghost::position mirror");

        }
        
        ghost::position ghost:: mirror_position() const throw()
        {
            return get_mirror_position( site );
        }
        
        ghost::position ghost:: get_lower_position( size_t dim )
        {
            switch( dim )
            {
                case 0: return at_lower_x;
                case 1: return at_lower_y;
                case 2: return at_lower_z;
                default: break;
            }
            critical_error(error_invalid_data, "invalid ghost::lower_position dimension index");
            return ghost::position(-1);
        }
        
        ghost::position ghost:: get_upper_position( size_t dim )
        {
            switch( dim )
            {
                case 0: return at_upper_x;
                case 1: return at_upper_y;
                case 2: return at_upper_z;
                default: break;
            }
            critical_error(error_invalid_data, "invalid ghost::upper_position dimension index");
            return ghost::position(-1);
        }


        
    }
}
