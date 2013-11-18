#ifndef YOCTO_IOS_STREAM_INCLUDED
#define YOCTO_IOS_STREAM_INCLUDED 1

#include "yocto/object.hpp"

namespace yocto
{
	namespace ios 
	{
		class stream : public virtual object
		{
		public:
			virtual ~stream() throw();
						
		protected:
			explicit stream() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(stream);
		};
		
	}
}

#endif
