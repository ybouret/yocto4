#ifndef YOCTO_IOS_IRCHANNEL_INCLUDED
#define YOCTO_IOS_IRCHANNEL_INCLUDED 1

#include "yocto/ios/ichannel.hpp"
#include "yocto/ios/raw-file.hpp"

namespace yocto
{
	namespace ios
	{
		
		class irchannel : public ichannel, public lockable
		{
		public:
			explicit irchannel( const string &filename, offset_t at=0, error_type *status=NULL );
			virtual ~irchannel() throw();
			virtual void get( void *data, size_t size, size_t &done );
			inline virtual void lock()     throw() { file_.lock(); }
			inline virtual void unlock()   throw() { file_.unlock(); }
			inline virtual bool try_lock() throw() { return file_.try_lock(); }
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(irchannel);
			raw_file file_;
			
		};
	}
	
}

#endif
