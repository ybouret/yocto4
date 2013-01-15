#ifndef PYCK_FILE_INCLUDED
#define PYCK_FILE_INCLUDED 1

#include "./mutex.hpp"
#include <cstdio>

class cFile
{
public:
    enum OpenMode
    {
        ReadOnly,
        Overwrite,
        Append
    };
    
    cFile( const char *filename, OpenMode mode);
    ~cFile() throw();
    
    void operator()(const char *fmt,...)  PYCK_PRINTF_CHECK(2,3);
    
    
    static Mutex Lock;
    
private:
    cFile( const cFile & );
    cFile&operator=(const cFile&);
    FILE *fp;
};

#endif
