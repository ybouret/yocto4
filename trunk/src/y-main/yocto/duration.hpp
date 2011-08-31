#ifndef YOCTO_DURATION_INCLUDED
#define YOCTO_DURATION_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{

	class duration 
	{
	public:
		static const unsigned max_days = 99;
		const unsigned d; //!< days
		const unsigned h; //!< hours
		const unsigned m; //!< minutes
		const double   s; //!< secondes
		
		duration( double seconds ) throw();
		~duration() throw();
		duration( const duration & ) throw();
		
	private:
		YOCTO_DISABLE_ASSIGN(duration);
	};
	
}

#endif
