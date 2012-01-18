#include "yocto/cliff/ghosts.hpp"

namespace yocto 
{
	
	namespace cliff
	{
		
		const char * ghost_position_label( ghost_position p ) throw()
		{
			switch( p )
			{
				case ghost_lower_x:
					return "lower x";
					
				case ghost_upper_x:
					return "upper x";
					
				case ghost_lower_y:
					return "lower y";
					
				case ghost_upper_y:
					return "upper y";
					
				case ghost_lower_z:
					return "lower z";
					
				case ghost_upper_z:
					return "upper z";
				
			}
			return "";
		}
		
		ghost_base:: ghost_base( size_t max_offsets, ghost_position pos ) :
		offsets(max_offsets,as_capacity),
		position( pos ),
		count(0),
		bytes(0)
		{
		}
		
		ghost_base:: ~ghost_base() throw()
		{
		}
		
		const char * ghost_base:: label() const throw()
		{
			return ghost_position_label(position);
		}
		
		
	}
	
	
}
