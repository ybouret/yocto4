#include "yocto/spade/ghosts/ghost.hpp"
#include "yocto/error.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // ghost
        //
        ////////////////////////////////////////////////////////////////////////
        ghost:: ~ghost() throw() {}
        
        ghost:: ghost( ghost::position p ) throw() :
        offsets_list(),
        site( p )
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
            return "";
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
                case at_lower_z: return at_upper_z;
                case at_upper_z: return at_lower_z;
            }
            critical_error(error_invalid_data, "invalid ghost::position mirror");
            return at_upper_x;
        }
        
        ghost::position ghost:: mirror_position() const throw()
        {
            return get_mirror_position( site );
        }
        
        ghost::position ghost:: get_lower_position( dimension_t dim )
        {
            switch( dim )
            {
                case on_x: return at_lower_x;
                case on_y: return at_lower_y;
                case on_z: return at_lower_z;
            }
            critical_error(error_invalid_data, "invalid ghost::lower_position dimension index");
            return ghost::position(-1);
        }
        
        ghost::position ghost:: get_upper_position( dimension_t dim )
        {
            switch( dim )
            {
                case on_x: return at_upper_x;
                case on_y: return at_upper_y;
                case on_z: return at_upper_z;
            }
            critical_error(error_invalid_data, "invalid ghost::upper_position dimension index");
            return ghost::position(-1);
        }
     
        void ghost::check( size_t dim, unit_t w, size_t ng)
        {
            if( size_t(w) < ng )
                throw exception("spade.ghost on %c: not enough width", char('x'+dim));
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // ghosts
        //
        ////////////////////////////////////////////////////////////////////////
        ghosts:: ~ghosts() throw() {}
        ghosts::  ghosts() throw() : num_offsets(0) {}
        
    }
    
}

