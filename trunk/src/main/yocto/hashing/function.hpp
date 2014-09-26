#ifndef YOCTO_HASHING_FUNCTION_INCLUDED
#define YOCTO_HASHING_FUNCTION_INCLUDED 1


#include "yocto/object.hpp"

namespace yocto
{
	
	namespace memory
	{
		class ro_buffer;
        class rw_buffer;
	}
	
	namespace hashing
	{
		
        //! base class for hashing function
		class function : public object
		{
		public:
			virtual ~function() throw();
			
			const size_t length; //!< output generation
			const size_t window; //!< internal window size
			
			virtual const char *name()   const throw() = 0;
			virtual void        set() throw() = 0; //!< initialize
			virtual void        run( const void *buffer, size_t buflen ) throw() = 0; //!< process bytes
			virtual void        get( void *output, size_t outlen ) throw() = 0;       //!< finalize/fill array
		
			void operator()( const void *buffer, size_t buflen ) throw(); //!< this->run(buffer,buflen);
			void operator()( const memory::ro_buffer &buf )      throw(); //!< this->run(buf.ro(),buf.length());
			void operator()( const char *buf )                   throw(); //!< this->run(buf,strlen(buf));
			
            template <typename T>
            void run_type( const T &x ) throw() { run(&x,sizeof(T)); }
            
            //! usually used by the virtual 'get' method.
			static void fill( void *output, size_t outlen, const void *input, size_t inlen ) throw();
			
			template <typename T>
			inline T key() throw() { T k(0); get( &k, sizeof(k) ); return k; }
			
            
            //! reset/run/key
			template <typename T>
			inline T key( const void *buffer, size_t buflen ) throw() { set(); run(buffer,buflen); return key<T>(); }
			
            //! reset/run/key
			template <typename T>
			inline T key( const memory::ro_buffer &buf ) throw() { set(); (*this)(buf); return key<T>(); }
			
            //!this->get( output.rw(), output.length() )
            void out( memory::rw_buffer &output ) throw();
            
            
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
