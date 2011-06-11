#ifndef YOCTO_HASHING_FUNCTION_INCLUDED
#define YOCTO_HASHING_FUNCTION_INCLUDED 1


#include "yocto/object.hpp"

namespace yocto
{
	
	namespace memory
	{
		class ro_buffer;
	}
	
	namespace hashing
	{
		
		class function : public object
		{
		public:
			virtual ~function() throw();
			
			const size_t length;
			const size_t window;
			
			virtual const char *name()   const throw() = 0;
			virtual void set() throw() = 0;
			virtual void run( const void *buffer, size_t buflen ) throw() = 0;
			virtual void get( void *output, size_t outlen ) throw() = 0;
			
			function & operator<<( const memory::ro_buffer & ) throw();
			function & operator<<( const char *) throw();
			
			static void fill( void *output, size_t outlen, const void *input, size_t inlen ) throw();
			
			template <typename T>
			inline T key() throw() { T k(0); get( &k, sizeof(k) ); return k; }
			
		protected:
			explicit function( size_t L, size_t W) throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(function);
		};
		
#define YOCTO_HASHING_FUNCTION_DECL(NAME,L,W) \
explicit NAME() throw();\
virtual ~NAME() throw();\
static const size_t __length = L ;\
static const size_t __window = W ;\
virtual const char *name()   const throw();\
virtual void set() throw();\
virtual void run( const void *buf, size_t len ) throw();\
virtual void get( void *output, size_t outlen ) throw()
	}
	
	
	
}


#endif
