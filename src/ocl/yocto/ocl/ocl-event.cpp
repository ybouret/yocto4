#include "yocto/ocl/event.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	namespace ocl
	{
		template <typename T, cl_event_info INFO>
		static inline
		T __GetEventInfo(cl_event      event,
						 const char   *tag)
		{
			assert( tag );
			T ans;
			const cl_int err = clGetEventInfo(event, INFO, sizeof(T), &ans, NULL);
			if( CL_SUCCESS != err )
				throw Exception( err, "clGetEventInfo(%s)", tag );
			return ans;
		}
		
		
		cl_command_queue Event::COMMAND_QUEUE( cl_event event )
		{
			return __GetEventInfo<cl_command_queue, CL_EVENT_COMMAND_QUEUE>( event, "CL_EVENT_COMMAND_QUEUE");
		}
		
		cl_command_type  Event::COMMAND_TYPE( cl_event event )
		{
			return __GetEventInfo<cl_command_type,CL_EVENT_COMMAND_TYPE>( event, "CL_EVENT_COMMAND_TYPE" );
		}
		
		cl_int Event::COMMAND_EXECUTION_STATUS( cl_event event )
		{
			return __GetEventInfo<cl_int,CL_EVENT_COMMAND_EXECUTION_STATUS>( event, "CL_COMMAND_EXECUTION_STATUS" );
		}
		
		static inline
		size_t _ref_count( cl_event event )
		{
			return __GetEventInfo<cl_uint,CL_EVENT_REFERENCE_COUNT>( event, "CL_EVENT_REFERENCE_COUNT" );
		}
		
		YOCTO_OPENCL_SHARED_IMPL(cl_event,clRetainEvent, clReleaseEvent, _ref_count);
		
		
		Event:: ~Event() throw() {}
		
		Event:: Event( cl_event event ) throw() : Shared<cl_event>( event ) {}
		
	}
	
}
