#ifndef YOCTO_THREADING_LAYOUT_INCLUDED
#define YOCTO_THREADING_LAYOUT_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
	
	namespace threading
	{
		
		class layout
		{
		public:
			const size_t size; //!< #threads requested
			const size_t root; //!< #CPU to start.
			explicit layout(); //!< parse the YOCTO_THREADING=#threads[,offset]"
			virtual ~layout() throw();
			layout( const layout &other ) throw();
			
		private:
			YOCTO_DISABLE_ASSIGN(layout);
		};
		
	}
	
}

#endif
