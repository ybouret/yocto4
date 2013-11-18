#include "yocto/ocl/event.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		Event::Profiling Event:: Profile() const
		{
			return Profiling( *(*this) );
		}
		
		
		
		
		
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

