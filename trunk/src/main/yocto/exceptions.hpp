//! \file exceptions.hpp

#ifndef YOCTO_EXCEPTIONS_INCLUDED
#define YOCTO_EXCEPTIONS_INCLUDED 1

#include "yocto/exception.hpp"

namespace yocto {


	namespace libc {

		//! wrapper for libc errors
		class exception : public yocto::exception
		{
		public:
			explicit exception( int err, const char *fmt, ... ) throw() YOCTO_PRINTF_CHECK(3,4);
			virtual ~exception() throw();
			exception( const exception & ) throw();

			virtual const char *what() const throw();
			int                 code() const throw();

		private:
			YOCTO_DISABLE_ASSIGN(exception);
			const int code_;
			char      what_[80];
		};

	}

	namespace win32 {

		//! wrapper for win32 errors
		class exception : public yocto::exception
		{
		public:
			explicit exception( uint32_t err, const char *fmt, ... ) throw() YOCTO_PRINTF_CHECK(3,4);
			virtual ~exception() throw();
			exception( const exception & ) throw();

			virtual const char *what() const throw();
			uint32_t            code() const throw();

		private:
			YOCTO_DISABLE_ASSIGN(exception);
			const uint32_t code_;
			char           what_[80];
		};
	}

#if defined(YOCTO_WIN)
	typedef win32::exception os_exception;
#endif
	
#if defined(YOCTO_BSD)
	typedef libc::exception    os_exception;
#endif
	
	namespace imported {

		//! base class for third party libraries error
		class exception : public yocto::exception
		{
		public:
			virtual ~exception() throw();
			explicit exception( const char *reason /*->what*/, const char *fmt /*->when*/,...) throw() YOCTO_PRINTF_CHECK(3,4);
			exception( const exception &other ) throw();
			exception & operator=( const yocto::exception &other ) throw();
			exception & operator=( const exception &other ) throw();
			
			virtual const char *what() const throw();
			
		private:
			char      what_[80];
		};

	}
}

#endif
