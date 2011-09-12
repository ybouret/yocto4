#ifndef YOCTO_OPENCL_TYPES_INCLUDED
#define YOCTO_OPENCL_TYPES_INCLUDED 1

#include "yocto/memory/buffers.hpp"
#include "yocto/memory/records.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/string.hpp"

#if defined(YOCTO_APPLE)
#	include <OpenCL/cl.h>
#else
#	include <CL/cl.h>
#endif

namespace yocto 
{
	namespace ocl
	{
		namespace Core
		{
			
			const char * StrErr( const cl_int err ) throw(); //!< convert errors to string
			
			typedef memory::pooled             MemoryType;    //!< memory allocator for buffers,strings...
			typedef MemoryType::allocator      Allocator;      
			//! C-Type array
			template <typename T>
			struct ArrayOf { typedef memory::buffer_of<T,MemoryType> Type; };
			
			//! C++-Type array, requires some memory
			template <typename T>
			struct RecordsOf { typedef memory::records_of<T> Type; };
			
			//! Parsed extensions
			typedef ArrayOf<char*>::Type Extensions;
			
			
			//! base class for auxiliary ref counted data
			class Object
			{
			public:
				explicit Object() throw();
				virtual ~Object() throw();
				
				void Retain()  throw(); //!< increase ref count
				bool Release() throw(); //!< decrease ref count, true if refcount==0
				const size_t ReferenceCount;
				
				
				static void * operator new( size_t block_size );
				static void   operator delete( void *p, size_t block_size );
				static void * operator new[]( size_t block_size );
				static void   operator delete[]( void *p, size_t block_size );
				
				
				//! smart reference counted pointer for classes derived from Object.
				template <typename T>
				class Proxy
				{
				public:
					YOCTO_ARGUMENTS_DECL_T;
					
					inline Proxy( T *pointee ) throw() : pointee_( pointee ) {
						assert( pointee_ );
						assert( 0 == pointee_->ReferenceCount );
						pointee_->Retain();
					}
					
					inline ~Proxy() throw()
					{
						assert( NULL != pointee_ );
						assert( pointee_->ReferenceCount > 0 );
						if( pointee_->Release() ) 
							delete pointee_;
						pointee_ = NULL;
					}
					
					inline Proxy( const Proxy &other ) throw() :
					pointee_( other.pointee_ )
					{
						assert( pointee_ );
						assert( pointee_->ReferenceCount > 0 );
						pointee_->Retain();
					}
					
					inline type &       operator*() throw()       { return *pointee_; }
					inline const_type & operator*() const throw() { return *pointee_; }
					
					inline type *       operator->() throw()       { return pointee_; }
					inline const_type * operator->() const throw() { return pointee_; }
					
				private:
					T *pointee_;
					YOCTO_DISABLE_ASSIGN(Proxy);
					
				};
				
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Object);
			};
			
			//! take care of OpenCL reference counted structs
			template <typename T>
			class Shared
			{
			public:
				YOCTO_ARGUMENTS_DECL_T;
				
				typedef cl_int (CL_API_CALL *Modifier)(T); //!< clRetainT|clReleaseT
				typedef size_t (            *RCounter)(T); //!< get reference count
				
				inline   Shared( T h ) throw() : handle_( h )
				{
					assert( NULL != handle_ );
				}
				
				inline   Shared( const Shared &other ) throw() : handle_( other.handle_ )
				{
					assert( NULL != handle_ );
					(void) Retain_( handle_ );
				}
				
				inline ~ Shared() throw()
				{
					assert( handle_ );
					(void) Release_(handle_);
				}
				
				inline type       & operator*() throw()       { assert(handle_); return handle_; }
				inline const_type & operator*() const throw() { assert(handle_); return handle_; }
				
				inline size_t ref_count() const { return RefCount_( handle_ ); }
				
			private:
				YOCTO_DISABLE_ASSIGN( Shared );
				T handle_;
				
				static const Modifier Retain_;   //!< to be set
				static const Modifier Release_;  //!< to be set
				static const RCounter RefCount_; //!< to be set
			};
			
#define YOCTO_OPENCL_SHARED_IMPL(CL_TYPE,RETAIN,RELEASE,REFCOUNT)                         \
/*   */ template <> const Shared<CL_TYPE>::Modifier Shared<CL_TYPE>::Retain_   = RETAIN;  \
/*   */ template <> const Shared<CL_TYPE>::Modifier Shared<CL_TYPE>::Release_  = RELEASE; \
/*   */ template <> const Shared<CL_TYPE>::RCounter Shared<CL_TYPE>::RefCount_ = REFCOUNT
			
			
			
			
		}
		
		//! base class for C++ wrapper classes
		class Class : public yocto::object
		{
		public:
			virtual ~Class() throw() {}
			explicit Class() throw() {}
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Class);
		};
		
	}
}

#endif
