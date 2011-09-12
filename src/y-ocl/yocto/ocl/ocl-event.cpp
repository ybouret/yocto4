#include "yocto/ocl/event.hpp"
#include "yocto/ocl/_query.hpp"

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
		
		
		namespace Core 
		{
			static inline
			size_t _ref_count( cl_event event )
			{
				return __GetEventInfo<cl_uint,CL_EVENT_REFERENCE_COUNT>( event, "CL_EVENT_REFERENCE_COUNT" );
			}
			
			YOCTO_OPENCL_SHARED_IMPL(cl_event,clRetainEvent, clReleaseEvent, _ref_count);
			
		}
		
		Event:: Event( cl_event event ) :
		hEvent_( event )
		{
		}
		
		Event:: ~Event() throw()
		{
			
		}
		
		Event::Profiling Event:: Profile() const
		{
			return Profiling( *hEvent_ );
		}

		
#if 0
		const cl_event & Event:: operator*() const throw()
		{
			return *hEvent_;
		}
#endif

		
#define YOCL_EVP(FIELD,TYPE) FIELD( Core::GetInfoValue( type2type<TYPE>(), clGetEventProfilingInfo, event, CL_PROFILING_##FIELD, "CL_PROFILING_" #FIELD ) )
		
		Event:: Profiling:: Profiling( cl_event event ) :
		YOCL_EVP(COMMAND_QUEUED,cl_ulong),
		YOCL_EVP(COMMAND_SUBMIT,cl_ulong),
		YOCL_EVP(COMMAND_START,cl_ulong),
		YOCL_EVP(COMMAND_END,cl_ulong)
		{
		}
		
		Event:: Profiling:: ~Profiling() throw()
		{
		}
		
		Event:: Profiling:: Profiling( const Profiling & other ) throw() :
		COMMAND_QUEUED( other.COMMAND_QUEUED ),
		COMMAND_SUBMIT( other.COMMAND_SUBMIT ),
		COMMAND_START(  other.COMMAND_START  ),
		COMMAND_END(    other.COMMAND_END    )
		{
		}
		
		cl_ulong Event:: Profiling:: LagNanoSeconds() const throw()
		{
			return COMMAND_SUBMIT - COMMAND_QUEUED;
		}
		
		cl_ulong Event:: Profiling:: RunNanoSeconds() const throw()
		{
			return COMMAND_END - COMMAND_START;
		}
		
		
		double Event:: Profiling:: LagTime() const throw()
		{
			return 1.0e-9 * double( LagNanoSeconds() );
		}
		
		double Event:: Profiling:: RunTime() const throw()
		{
			return 1.0e-9 * double( RunNanoSeconds() );
		}
		
	}
	
}
