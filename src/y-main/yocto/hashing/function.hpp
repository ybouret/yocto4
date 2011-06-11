#ifndef YOCTO_HASHING_FUNCTION_INCLUDED
#define YOCTO_HASHING_FUNCTION_INCLUDED 1


#include "yocto/object.hpp"

namespace yocto
{

	namespace hashing
	{

		class function : public object
		{
		public:
			virtual ~function() throw();

			const size_t length;
			const size_t window;

			virtual const char *name()   const throw() = 0;
			virtual void init() throw() = 0;
			virtual void update( const void *buf, size_t len ) throw() = 0;
			virtual void final() throw() = 0;


			static void fill( void *buffer, size_t buflen, const void *output, size_t outlen ) throw();

		protected:
			explicit function( size_t L, size_t W) throw();

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(function);
		};

#define YOCTO_HASHING_FUNCTION_DECL(L,W) \
	static const size_t __length = L ;\
	static const size_t __window = W ;\
	virtual const char *name()   const throw();\
	virtual void init() throw();\
	virtual void update( const void *buf, size_t len ) throw();\
	virtual void final() throw()
	}



}


#endif
