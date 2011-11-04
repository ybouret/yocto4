#include "yocto/duration.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>

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
		static const unsigned seconds_per_minute = 60;
		static const unsigned seconds_per_hour   = 60 * seconds_per_minute;
		static const unsigned seconds_per_day    = 24 * seconds_per_hour;
		static const unsigned max_seconds        = max_days * seconds_per_day;
		seconds = fabs(seconds);
		if( seconds >= max_seconds )
		{
			(unsigned &)d = max_days;
		}
		else 
		{
			double         intpart = 0;
			const double   frcpart = modf( seconds, &intpart );
			unsigned       num_seconds = unsigned( intpart );
			(unsigned&)d = num_seconds / seconds_per_day;
			num_seconds -= d * seconds_per_day;
			(unsigned&)h = num_seconds / seconds_per_hour;
			num_seconds -= h * seconds_per_hour;
			(unsigned&)m = num_seconds / seconds_per_minute;
			num_seconds -= m * seconds_per_minute;
			(double&)s   = static_cast<double>(num_seconds) + frcpart;
		}

		
		
	}
	
	
	std::ostream & operator<<( std::ostream &os, const duration &D )
	{
		os << /* std::setw(2) << */ D.d << "D+";
		os << /* std::setw(2) << */ D.h << 'H';
		os << /* std::setw(2) << */ D.m << 'M';
		os << /* std::setw(2) << */ D.s;
		return os;
	}
	
}
