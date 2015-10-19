#ifndef YOCTO_RXX_INCLUDED
#define YOCTO_RXX_INCLUDED 1

#include "yocto/R/IR.hpp"

#if defined(YOCTO_WIN)
#undef ERROR
#endif

#include <R.h>
#include <Rinternals.h>

#include "yocto/exception.hpp"

namespace yocto
{

#define YOCTO_R_PROLOG() try {

#define YOCTO_R_EPILOG() } \
catch(const yocto::exception &e) { \
Rprintf("\t*** %s\n",e.what());\
Rprintf("\t*** %s\n",e.when());\
return R_NilValue;\
}\
catch(...) { Rprintf("\t*** Unhandled exception !\n"); return R_NilValue; }



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
        static inline double * Cast( SEXP Rvec ) { assert(Rvec); return REAL(Rvec); }
        enum { Conv = REALSXP };
    };
    
    //! for INT
    template <>
    struct RGetData<int>
    {
        static inline int * Cast( SEXP Rvec ) { assert(Rvec); return INTEGER(Rvec); }
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
        explicit RObject() throw() : is_R(false) {}
        inline void set_R() throw() { (bool&)is_R = true; }
        virtual SEXP get_SEXP() const throw() = 0;
        
        
    private:
        RObject( const RObject &);
        RObject&operator=(const RObject &);
    };
    

    //__________________________________________________________________________
    //
    //
    //! A R vector, 0 based
    //
    //__________________________________________________________________________
    template <typename T>
    class RVector : public RObject, public RArray<T>
    {
    public:
        virtual ~RVector() throw()
        {
        }
        
        
        //! get data from R
        explicit RVector( SEXP args ) :
        RObject(),
        RArray<T>(),
        Rvec( coerceVector(args, RGetData<T>::Conv) ),
        data( RGetData<T>::Cast(Rvec) ),
        items( length(Rvec) )
        {
        }

        //! set data for R
        explicit RVector( size_t n ) :
        RObject(),
        Rvec( allocVector(RGetData<T>::Conv,n) ),
        data( RGetData<T>::Cast(Rvec) ),
        items( length(Rvec) )
        {
            PROTECT(Rvec);
            set_R();
        }
        
        YOCTO_R_ARRAY_IMPL(data,items)

    private:
        RVector(const RVector &);
        RVector&operator=(const RVector &);
        SEXP         Rvec;
        T           *data;
        virtual SEXP get_SEXP() const throw() { return Rvec; }
        
    public:
        const size_t items;
        
    };
    
    //__________________________________________________________________________
    //
    //
    //! R matrix, columns major
    //
    //__________________________________________________________________________
    template <typename T>
    class RMatrix : public RObject, public Matrix<T>
    {
    public:
        class Column : public RArray<T>
        {
        public:
            inline Column( T *p, size_t r) throw() :
            data(p),
            rows(r)
            {
            }
            

            YOCTO_R_ARRAY_IMPL(data,rows)
        private:
            T *data;
            Column( const Column & );
            Column&operator=(const Column &);
            virtual ~Column() throw() {}
        public:
            const size_t rows;
        };
        
        
        
        virtual ~RMatrix() throw() { operator delete(mcol); mcol=0; }

        //! get data from R
        explicit RMatrix( SEXP args ) :
        Rmat( coerceVector(args, RGetData<T>::Conv) ),
        data( RGetData<T>::Cast(Rmat) ),
        mcol(0)
        {
            get_dims();
        }

        //! set data for R
        explicit RMatrix( size_t r, size_t c ) :
        Rmat( allocMatrix( RGetData<T>::Conv, r, c) ),
        data( RGetData<T>::Cast(Rmat) ),
        mcol(0)
        {
            PROTECT(Rmat);
            set_R();
            get_dims();
        }
        
        inline Column &       operator[](size_t c) throw()       { assert(c<this->cols); return mcol[c]; }
        inline const Column & operator[](size_t c) const throw() { assert(c<this->cols); return mcol[c]; }

        inline virtual T & operator()(size_t ir, size_t ic) throw()
        {
            return (*this)[ic][ir];
        }

        inline virtual const T & operator()(size_t ir, size_t ic) const throw()
        {
            return (*this)[ic][ir];
        }

    private:
        
        RMatrix(const RMatrix&);
        RMatrix&operator=(const RMatrix &);
        virtual SEXP get_SEXP() const throw() { return Rmat; }
        
        SEXP    Rmat;
        T      *data;
        Column *mcol;
        
        inline void get_dims()
        {
            SEXP             Rdim   = getAttrib(Rmat, R_DimSymbol);
            this->setDimensions(INTEGER(Rdim)[0],INTEGER(Rdim)[1]);
            mcol = static_cast<Column *>( operator new( (this->cols)*sizeof(Column) ) );
            T *p = data;
            for( size_t i=0; i < this->cols; ++i, p += this->rows )
            {
                new (mcol+i) Column(p,this->rows);
            }
        }
    };


    //__________________________________________________________________________
    //
    //
    //! CMatrix directly built from R matrix
    //
    //__________________________________________________________________________
    template <typename T>
    class CMatrixR : public CMatrix<T>
    {
    public:
        inline explicit CMatrixR( SEXP args ) :
        CMatrix<T>( get_dim(0,args), get_dim(1,args) )
        {
            // get data from R
            SEXP     Rmat = coerceVector(args, RGetData<T>::Conv);
            const T *p    = RGetData<T>::Cast(Rmat);

            // make a copy
            const size_t r = this->rows;
            const size_t c = this->cols;
            for(size_t i=0;i<r;++i)
            {
                const size_t offset = i*c;
                for(size_t j=0;j<c;++j)
                {
                    this->data[offset+j] = p[i+r*j];
                }
            }

        }

        inline virtual ~CMatrixR() throw()
        {
        }

        inline void print() const
        {
            for(size_t i=0; i < this->rows; ++i )
            {
                for(size_t j=0;j<this->cols;++j)
                    Rprintf(" %g", (*this)[i][j]);
                Rprintf("\n");
            }
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(CMatrixR);
        static inline int get_dim(size_t i, SEXP args)
        {
            SEXP Rmat = coerceVector(args, RGetData<T>::Conv);
            SEXP Rdim = getAttrib(Rmat, R_DimSymbol);
            return INTEGER(Rdim)[i];
        }
    };

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
        RList(const RArray<const char*> &names) :
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

    //__________________________________________________________________________
    //
    //! argument conversion, ok for double and int
    //__________________________________________________________________________
    template <typename T>
    inline T R2Scalar( SEXP r )
    {
        const RVector<T> value(r);
        if( value.size() < 1) throw exception("R2Scalar: invalid argument");
        return value[0];
    }

    //__________________________________________________________________________
    //
    //! string conversion
    //__________________________________________________________________________
    inline const char *R2String( SEXP r )
    {
        return CHAR(STRING_ELT(r,0));
    }
    
    
}

#endif
