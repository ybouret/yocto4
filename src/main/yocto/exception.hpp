//! \file exception.hpp
/**
	\brief exception with context
 */
#ifndef YOCTO_EXCEPTION_INCLUDED
#define YOCTO_EXCEPTION_INCLUDED 1

#include "yocto/os.hpp"
#include "yocto/code/printf-check.hpp"
#include <exception>

namespace yocto
{
	//! base class for exception
	class exception : public std::exception
	{
	public:
		static const size_t max_length = 120;
		
		virtual ~exception() throw();
		exception( const exception &other ) throw();
		
		virtual const char *what() const throw(); //!< overrides std::exception::what()
		const char *when() const throw();         //!< exception context
		
		explicit exception( const char *fmt,...) throw() YOCTO_PRINTF_CHECK(2,3); //!< format when
		void     cat( const char *fmt,...)       throw() YOCTO_PRINTF_CHECK(2,3); //!< append to when
		void     set( const char *fmt,...)       throw() YOCTO_PRINTF_CHECK(2,3); //!< reset when
		void     hdr( const char *fmt,...)       throw() YOCTO_PRINTF_CHECK(2,3); //!< prepend to when
		
	protected:
		explicit exception() throw();
		void     format( const char *fmt, void *va_list_ptr) throw();
		char     when_[ max_length ];
		
	private:
		YOCTO_DISABLE_ASSIGN(exception);
		
	};
	
}

#define YOCTO_FAILSAFE(CODE,FINALIZE) do { try { CODE; FINALIZE; }catch(...){ FINALIZE; throw; } } while(false)


#endif
