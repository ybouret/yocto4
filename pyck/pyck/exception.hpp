#ifndef PYCK_EXECPTION_INCLUDED
#define PYCK_EXCEPTION_INCLUDED 1

#include <exception>
#include <cstdlib>

class Exception : public std::exception
{
public:
    static const size_t Length = 128;
    
    Exception() throw();
    Exception( const Exception & ) throw();
    virtual ~Exception() throw();
    
    virtual const char * what() const throw();
    
private:
    Exception & operator=( const Exception & );
    char what_[ Length ];
};

#endif

