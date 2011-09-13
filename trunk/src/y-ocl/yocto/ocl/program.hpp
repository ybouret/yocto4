#ifndef YOCTO_OCL_PROGRAM_INCLUDED
#define YOCTO_OCL_PROGRAM_INCLUDED 1

#include "yocto/ocl/codes.hpp"
#include "yocto/ocl/context.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		class Driver;
		
		class Program : public Shared<cl_program>
		{
		public:
			explicit Program( const Context &ctx, const Sources &src );
			virtual ~Program() throw();
		
			
		private:
			DeviceIDs            deviceIDs;   //!< max devices from context
			DeviceMap            devices;     //!< valid devices
		public:                               //
			const size_t       & NUM_DEVICES; //!< CL_PROGRAM_NUM_DEVICES
			const cl_device_id * DEVICES;     //!< CL_PROGRAM_DEVICES
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Program);
			friend class Driver;
		};
		
	}
	
}

#endif
