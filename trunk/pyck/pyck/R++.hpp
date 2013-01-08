#ifndef PYCK_R_INCLUDED
#define PYCK_R_INCLUDED 1


#include "./exception.hpp"

#include <R.h>
#include <Rinternals.h>



template <typename T> struct RGetData;

template <>
struct RGetData<double>
{
    static inline double * Cast( SEXP Rvec ) { return REAL(Rvec); }
    enum { Conv = REALSXP };
};

template <>
struct RGetData<int>
{
    static inline int * Cast( SEXP Rvec ) { return INTEGER(Rvec); }
    enum { Conv = INTSXP };
};


template <typename T>
class RVector
{
public:
    virtual ~RVector() throw()
    {
        if(is_R)  { UNPROTECT(1); }
    }
    
    inline T       & operator[](size_t indx) throw()       { assert(indx<size); return data[indx]; }
    inline const T & operator[](size_t indx) const throw() { assert(indx<size); return data[indx]; }
   
    explicit RVector( SEXP args ) :
    Rvec( coerceVector(args, RGetData<T>::Conv) ),
    data( RGetData<T>::Cast(Rvec) ),
    is_R( false ),
    size( length(Rvec) )
    {
    }
    
    explicit RVector( size_t n ) :
    Rvec( allocVector(RGetData<T>::Conv,n) ),
    data( RGetData<T>::Cast(Rvec) ),
    is_R( true ),
    size( length(Rvec) )
    {
        PROTECT(Rvec);
    }
    
    inline SEXP operator*() const throw() { return Rvec; }
        
private:
    RVector(const RVector &);
    RVector&operator=(const RVector &);
    SEXP       Rvec;
    T         *data;
    const bool is_R;
    
public:
    const size_t size;
    
};



#endif
