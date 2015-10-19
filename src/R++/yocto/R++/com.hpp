#ifndef YOCTO_RXX_COM_INCLUDED
#define YOCTO_RXX_COM_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/exception.hpp"

#if defined(YOCTO_WIN)
#undef ERROR
#endif

#include <R.h>
#include <Rinternals.h>


namespace yocto
{

    //__________________________________________________________________________
    //
    //
    // static and export functions
    //
    //__________________________________________________________________________
#define YOCTO_R_EXPORT extern "C"
#define YOCTO_R_STATIC static inline

    //__________________________________________________________________________
    //
    //
    // !Wrappers to R function implementation
    //
    /**
     YOCTO_R_FUNCTION(myfunc,(SEXP a,...))
     {
     }
     YOCTO_R_RETURN()
     */
    //__________________________________________________________________________
#define YOCTO_R_FUNCTION(NAME,ARGS) YOCTO_R_EXPORT SEXP NAME ARGS throw()\
{ static const char __fn[] = #NAME; \
try

#define YOCTO_R_RETURN() \
catch(const yocto::exception &e) { \
Rprintf("\t***[%s] %s\n", __fn, e.what());\
Rprintf("\t***[%s] %s\n", __fn, e.when());\
return R_NilValue;\
}\
catch(...) {\
Rprintf("\t***[%s] Unhandled exception!");\
return R_NilValue; }\
}

    //__________________________________________________________________________
    //
    //
    // Data Types conversion
    //
    //__________________________________________________________________________

    //! template for R data type handling
    template <typename T> struct RGetData;

    //! for REAL
    template <>
    struct RGetData<double>
    {
        static inline double * Cast( SEXP &Rvec ) { assert(Rvec); return REAL(Rvec); }
        enum { Conv = REALSXP };
    };

    //! for INT
    template <>
    struct RGetData<int>
    {
        static inline int * Cast( SEXP &Rvec ) { assert(Rvec); return INTEGER(Rvec); }
        enum { Conv = INTSXP };
    };

    //__________________________________________________________________________
    //
    //
    // access properties
    //
    //__________________________________________________________________________


    //__________________________________________________________________________
    //
    //
    // Base class to communicate with R memory
    //
    //__________________________________________________________________________

    //! R Object
    class RObject
    {
    public:
        const bool is_R; //!< handled by R, return value
        virtual ~RObject() throw() { if(is_R)  { UNPROTECT(1); } }

        inline SEXP operator*() const throw() { return get_SEXP(); }


    protected:
        inline explicit RObject() throw() : is_R(false) {}
        inline void set_R() throw() { (bool&)is_R = true; }
        virtual SEXP get_SEXP() const throw() = 0;


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(RObject);
    };

    //__________________________________________________________________________
    //
    //
    //! Base class for Rvector
    //
    //__________________________________________________________________________

    class _RVector : public RObject
    {
    public:
        inline virtual ~_RVector() throw() {}

    protected:
        inline _RVector() throw() : RObject() {}
        inline void        onFree()    const throw() { Rprintf("%s: meaningless free!",onName());    }
        inline void        onRelease() const throw() { Rprintf("%s: meaningless release!",onName()); }
        inline const char *onName()    const throw() { return "RVector"; }
        inline void        onReserve()               { throw exception("%s: no reserve!",onName());  }

        inline  SEXP checkSEXP(SEXP v) const
        {
            if(!v) throw exception("%s: invalid SEXP!",onName());
            return v;
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(_RVector);
    };

    //__________________________________________________________________________
    //
    //
    //! A R vector, 1 based
    //
    //__________________________________________________________________________
    template <typename T>
    class RVector : public _RVector, public array<T>
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        virtual ~RVector() throw()
        {
        }


        //! get data from R
        explicit RVector( SEXP args ) :
        _RVector(),
        Rvec(  checkSEXP(coerceVector(args, RGetData<T>::Conv)) ),
        data_( RGetData<T>::Cast(Rvec)-1),
        size_( length(Rvec) )
        {
        }

        //! set data for R
        explicit RVector( size_t n ) :
        _RVector(),
        Rvec(  checkSEXP(allocVector(RGetData<T>::Conv,n)) ),
        data_( RGetData<T>::Cast(Rvec)-1),
        size_( length(Rvec) )
        {
            PROTECT(Rvec);
            set_R();
        }

        
        inline virtual size_t      size()     const throw() { return size_;    }
        inline virtual size_t      capacity() const throw() { return size_;    }
        inline virtual void        free()     throw()       {  onFree();       }
        inline virtual void        release()  throw()       {  onRelease();    }
        inline virtual void        reserve(size_t)          {  onReserve();    }
        inline virtual const char *name() const throw()     { return onName(); }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(RVector);
        SEXP         Rvec;
        T           *data_;
        const size_t size_;
        inline virtual SEXP get_SEXP() const throw() { return Rvec; }
        inline virtual const_type *get_item() const throw() { return data_; }
    };


}


#endif
