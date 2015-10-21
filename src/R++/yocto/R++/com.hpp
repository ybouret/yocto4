#ifndef YOCTO_RXX_COM_INCLUDED
#define YOCTO_RXX_COM_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/lw-array.hpp"

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


    //__________________________________________________________________________
    //
    //
    // R Matrix, Column Major
    //
    //__________________________________________________________________________
    template <typename T>
    class RMatrix : public RObject, public matrix_of<T>
    {
    public:
        class Column : public lw_array<T>
        {
        public:
            inline Column(T *ptr, size_t num) throw() : lw_array<T>(ptr,num) {}
            inline virtual ~Column() throw() {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Column);
        };

        Column & operator[](size_t ic) throw()
        {
            assert(ic>0);
            assert(ic<=this->cols);
            return mcol[ic];
        }

        const Column & operator[](size_t ic) const throw()
        {
            assert(ic>0);
            assert(ic<=this->cols);
            return mcol[ic];
        }


        inline virtual ~RMatrix() throw()
        {
            memory::kind<memory::global>::release_as<Column>(++mcol,cnum);
        }

        inline explicit RMatrix(SEXP args) :
        RObject(),
        Rmat( coerceVector(args, RGetData<T>::Conv) ),
        data( RGetData<T>::Cast(Rmat) ),
        mcol(0),
        cnum(0)
        {
            setDims();
        }



        //! set data for R
        inline explicit RMatrix( size_t r, size_t c ) :
        Rmat( allocMatrix( RGetData<T>::Conv, r, c) ),
        data( RGetData<T>::Cast(Rmat) ),
        mcol(0),
        cnum(0)
        {
            PROTECT(Rmat);
            set_R();
            setDims();
        }




    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(RMatrix);
        SEXP     Rmat;
        T       *data;
        Column  *mcol;
        size_t   cnum;

        inline void setDims()
        {
            // get dimensions
            SEXP         Rdim = getAttrib(Rmat, R_DimSymbol);
            const size_t nr   = INTEGER(Rdim)[0];
            const size_t nc   = INTEGER(Rdim)[1];
            const size_t ni   = nr*nc;

            // get columns
            cnum = nc;
            mcol = memory::kind<memory::global>::acquire_as<Column>(cnum)-1;

            // link
            T *p = data;
            for(size_t i=1;i<=nc;++i,p+=nr)
            {
                new (mcol+i) Column(p,nr);
            }

            (size_t &)(this->rows)  = nr;
            (size_t &)(this->cols)  = nc;
            (size_t &)(this->items) = ni;

        }

        virtual SEXP get_SEXP() const throw()
        {
            return Rmat;
        }

        virtual void *address_of(size_t ir,size_t ic) const throw()
        {
            assert(ir>0);assert(ir<=this->rows);
            assert(ic>0);assert(ic<=this->cols);
            const RMatrix<T> &self = *this;
            return (void*)&self[ic][ir];
        }

        virtual void *address_of(size_t it) const throw()
        {
            assert(it<this->items);
            return data+it;
        }
        
    };



    //__________________________________________________________________________
    //
    //! argument conversion, ok for double and int
    //__________________________________________________________________________
    template <typename T>
    inline T R2Scalar( SEXP r )
    {
        const RVector<T> value(r);
        if( value.size() < 1) throw exception("R2Scalar: invalid argument");
        return value[1];
    }

    //__________________________________________________________________________
    //
    //! string conversion
    //__________________________________________________________________________
    inline const char *R2String( SEXP r )
    {
        return CHAR(STRING_ELT(r,0));
    }


    //__________________________________________________________________________
    //
    //
    //! RList wrapper
    //
    //__________________________________________________________________________
    class RList : public RObject
    {
    public:

        //! create a list for R
        RList(const char *names[], const size_t count) :
        size( check_list_size(count) ),
        L( allocVector(VECSXP,size)  )
        {
            PROTECT(L);

            //-- set list items name
            SEXP list_names = 0;
            PROTECT(list_names = allocVector(STRSXP,size));
            for(size_t i = 0; i < size; i++)
            {
                SET_STRING_ELT(list_names,i,mkChar(names[i]));
            }
            setAttrib(L, R_NamesSymbol, list_names);
            UNPROTECT(1);
            //-- list_name

            set_R();
        }

        //! create a list for R
        RList(const array<const char*> &names) :
        size( check_list_size(names.size()) ),
        L(allocVector(VECSXP,size))
        {
            PROTECT(L);

            //-- set list items name
            SEXP list_names = 0;
            PROTECT(list_names = allocVector(STRSXP,size));
            for(size_t i = 0; i < size; i++)
            {
                SET_STRING_ELT(list_names,i,mkChar(names[i]));
            }
            setAttrib(L, R_NamesSymbol, list_names);
            UNPROTECT(1);
            //-- list_name

            set_R();

        }

        virtual ~RList() throw() {}

        inline void set(size_t indx, const RObject &elmt )
        {
            assert(indx<size);
            SET_VECTOR_ELT(L, indx, *elmt);
        }

        const size_t size;

    private:
        SEXP         L;

        virtual SEXP get_SEXP() const throw() { return L; }

        RList(const RList &);
        RList&operator=(const RList &);
        static inline
        size_t check_list_size(size_t count)
        {
            if(count<=0) throw exception("Empty RList creation!");
            return count;
        }
    };



}


#endif
