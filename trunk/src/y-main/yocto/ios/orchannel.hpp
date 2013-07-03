#ifndef YOCTO_IOS_ORCHANNEL_INCLUDED
#define YOCTO_IOS_ORCHANNEL_INCLUDED 1

#include "yocto/ios/ochannel.hpp"
#include "yocto/ios/raw-file.hpp"

namespace yocto
{
	namespace ios
	{
		
		class orchannel : public ochannel, public lockable
		{
		public:
			explicit orchannel( const string &filename, offset_t at=0, error_type *status=NULL );
            explicit orchannel( const char   *filename, offset_t at=0, error_type *status=NULL );
            explicit orchannel( raw_file::handle_t handle, error_type *status = NULL); //!< for pipe
			virtual ~orchannel() throw();
			virtual void put( const void *data, size_t size, size_t &done );
			inline virtual void lock()     throw() { file_.lock(); }
			inline virtual void unlock()   throw() { file_.unlock(); }
			inline virtual bool try_lock() throw() { return file_.try_lock(); }
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(orchannel);
			raw_file file_;
			
		};
	}
	
}

#endif
