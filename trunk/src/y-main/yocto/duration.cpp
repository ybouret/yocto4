#include "yocto/duration.hpp"

namespace yocto
{

	duration:: duration( const duration &other ) throw() :
	d( other.d ),
	h( other.h ),
	m( other.m ),
	s( other.s )
	{
	}
	
	duration:: ~duration() throw() {}
	
	duration:: duration( double seconds ) throw() :
	d( 0 ),
	h( 0 ),
	m( 0 ),
	s( 0 )
	{
		static const double seconds_per_minute = 60;
		static const double seconds_per_hour   = 60 * seconds_per_minute;
		static const double seconds_per_day    = 24 * seconds_per_hour;
		static const double max_seconds        = max_days * seconds_per_day;
		
		if( seconds >= max_seconds )
		{
			(unsigned &)d = max_days;
		}
		else 
		{
				
		}

		
		
	}
	
}
