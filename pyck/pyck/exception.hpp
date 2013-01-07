#ifndef PYCK_EXECPTION_INCLUDED
#define PYCK_EXCEPTION_INCLUDED 1

#include "./os.hpp"
#include <exception>

class Exception : public std::exception
{
public:
    static const size_t Length = 128;
    
    Exception(const char *fmt,...) throw() PYCK_PRINTF_CHECK(2,3);
    Exception( const Exception & ) throw();
    virtual ~Exception() throw();
    
    virtual const char * what() const throw();
    
private:
    Exception & operator=( const Exception & );
    char what_[ Length ];
};

#endif

