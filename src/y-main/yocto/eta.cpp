#include "yocto/eta.hpp"
#include "yocto/duration.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	
	static const double max_seconds = duration::max_days * ( 24 * 60 * 60 );
	
	eta:: ~eta() throw() {}
	
	
	eta:: eta() : 
	time_done(0),
	time_left(max_seconds),
	ratio_done(0),
	ratio_left(1),
	chrono_()
	{
		chrono_.start();
	}
	
	void eta:: reset()
	{
		(double &)time_done  = 0;
		(double &)time_left  = max_seconds;
		(double &)ratio_done = 0;
		(double &)ratio_left = 1;
		chrono_.start();
	}
	
	double eta:: now()
	{
		return chrono_.query();
	}
	
	void   eta:: operator()( double ratio )
	{
		(double &)ratio_done = clamp<double>(0,ratio,1);
		(double &)ratio_left = 1.0 - ratio_done;
		(double &)time_done  = now();
		const double num = ratio_left * time_done;
		if( num >= ratio_done * max_seconds )
			(double &)time_left = max_seconds;
		else 
			(double &)time_left = num / ratio;
	}
}
