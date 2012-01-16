#ifndef YOCTO_CODEC_INCLUDED
#define YOCTO_CODEC_INCLUDED 1


#include "yocto/ios/ostream.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace codec
	{
		typedef functor<void,TL1(char)> writer;
		
		//! generic interface
		/**
			the write function MUST be resilient to exceptions...
		 */
		class filter : public ios::ostream
		{
		public:
			virtual ~filter() throw();
			virtual const char * name() const throw() = 0;
			
			writer output; //!< to be used by algortihm
			
		protected:
			explicit filter( const writer &out );
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(filter);
		};
		
	}
		
}

#endif
