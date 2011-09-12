#ifndef YOCTO_OPENCL_EVENT_INCLUDED
#define YOCTO_OPENCL_EVENT_INCLUDED 1

#include "yocto/ocl/types.hpp"
#include "yocto/container/container.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		//! a simple cl_event shared pointer
		class Event
		{
		public:
			//! take car of event, no extra retain
			explicit Event( cl_event event ); 
			virtual ~Event() throw();
			Event( const Event & ) throw(); //!< shared copy
			
			
			inline cl_command_queue COMMAND_QUEUE( ) const
			{
				return COMMAND_QUEUE( *hEvent_ );
			}
			
			inline cl_command_type COMMAND_TYPE() const
			{
				return COMMAND_TYPE( *hEvent_ );
			}
			
			inline cl_int  COMMAND_EXECUTION_STATUS() const
			{
				return COMMAND_EXECUTION_STATUS( *hEvent_ );
			}
			
		
		private:
			Core::Shared<cl_event> hEvent_;
			YOCTO_DISABLE_ASSIGN(Event);
			
		public:
			
			static cl_command_queue COMMAND_QUEUE( cl_event );
			static cl_command_type  COMMAND_TYPE(  cl_event );
			static cl_int           COMMAND_EXECUTION_STATUS( cl_event );
			
			class Profiling
			{
			public:
				Profiling( cl_event );
				~Profiling() throw();
				
				const cl_ulong COMMAND_QUEUED;
				const cl_ulong COMMAND_SUBMIT;
				const cl_ulong COMMAND_START;
				const cl_ulong COMMAND_END;
				
				Profiling( const Profiling & ) throw();
				
				cl_ulong LagNanoSeconds() const throw();
				cl_ulong RunNanoSeconds() const throw();
				double   LagTime() const throw(); //!< in seconds
				double   RunTime() const throw(); //!< in seconds
				
			private:
				YOCTO_DISABLE_ASSIGN(Profiling);
			};
			
			Profiling Profile() const;
			
			class List : public container
			{
			public:
				explicit List() throw();
				explicit List( size_t );
				virtual ~List() throw();
				List( const List & ); //!< copy with extra retain
				
				virtual const char *name() const throw();
				
				virtual void   reserve( size_t n );
				virtual size_t size()     const throw();
				virtual size_t capacity() const throw();
				virtual void   free()     throw();
				virtual void   release()  throw();
				
				void insert( const cl_event ); //!< raw insert, no extra retain, released upon failure
				void insert( const Event &  ); //!< raw insert, extra retain upon success
				
				inline size_t           num_events(void) const throw() { return size_; }
				inline const cl_event * operator()(void) const throw() { return size_ ? event_ : NULL; }
				
				void WaitFor();
				
			private:
				size_t              size_;
				size_t              maxi_;
				cl_event           *event_;
			
				void do_reserve_( size_t n );
				bool look_up( const cl_event, size_t &index ) const throw();
				
				YOCTO_DISABLE_ASSIGN(List);
			};
			
		};
		
		
	}
	
}

#endif
