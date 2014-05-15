#ifndef YOCTO_RXX_INCLUDED
#define YOCTO_RXX_INCLUDED 1

#include <R.h>
#include <Rinternals.h>
#include "yocto/exception.hpp"

namespace yocto
{
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
    
    //! R vector
    template <typename T>
    class RVector : public RObject
    {
    public:
        virtual ~RVector() throw()
        {
        }
        
        inline T       & operator[](size_t indx) throw()       { assert(indx<size); return data[indx]; }
        inline const T & operator[](size_t indx) const throw() { assert(indx<size); return data[indx]; }
        
        explicit RVector( SEXP args ) :
        RObject(),
        Rvec( coerceVector(args, RGetData<T>::Conv) ),
        data( RGetData<T>::Cast(Rvec) ),
        size( length(Rvec) )
        {
        }
        
        explicit RVector( size_t n ) :
        RObject(),
        Rvec( allocVector(RGetData<T>::Conv,n) ),
        data( RGetData<T>::Cast(Rvec) ),
        size( length(Rvec) )
        {
            PROTECT(Rvec);
            set_R();
        }
        
    private:
        RVector(const RVector &);
        RVector&operator=(const RVector &);
        SEXP         Rvec;
        T           *data;
        virtual SEXP get_SEXP() const throw() { return Rvec; }
        
    public:
        const size_t size;
        
    };
    
    //! Core Matrix
    class CoreMatrix
    {
    public:
        const size_t rows;
        const size_t cols;
        const size_t items;
        
        virtual ~CoreMatrix() throw()
        {
            (size_t&)rows  = 0;
            (size_t&)cols  = 0;
            (size_t&)items = 0;
        }
        
    protected:
        explicit CoreMatrix() throw() :
        rows(0), cols(0), items(0)
        {}
        
    private:
        CoreMatrix(const CoreMatrix &);
        CoreMatrix&operator=(const CoreMatrix&);
    };
    
    //! R matrix, columns major
    template <typename T>
    class RMatrix : public RObject, public CoreMatrix
    {
    public:
        class Column
        {
        public:
            inline Column( T *p, size_t r) throw() :
            data(p),
            rows(r)
            {
            }
            
            inline T  & operator[]( size_t r ) throw()          { assert(r<rows); return data[r]; }
            inline const T & operator[](size_t r) const throw() { assert(r<rows); return data[r]; }
            
        private:
            T *data;
            Column( const Column & );
            Column&operator=(const Column &);
            ~Column() throw();
        public:
            const size_t rows;
        };
        
        
        
        virtual ~RMatrix() throw() { operator delete(mcol); mcol=0; }
        
        explicit RMatrix( SEXP args ) :
        Rmat( coerceVector(args, RGetData<T>::Conv) ),
        data( RGetData<T>::Cast(Rmat) ),
        mcol(0)
        {
            get_dims();
        }
        
        explicit RMatrix( size_t r, size_t c ) :
        Rmat( allocMatrix( RGetData<T>::Conv, r, c) ),
        data( RGetData<T>::Cast(Rmat) ),
        mcol(0)
        {
            PROTECT(Rmat);
            set_R();
            get_dims();
        }
        
        inline Column &       operator[](size_t c) throw()       { assert(c<cols); return mcol[c]; }
        inline const Column & operator[](size_t c) const throw() { assert(c<cols); return mcol[c]; }
        
    private:
        
        RMatrix(const RMatrix&);
        RMatrix&operator=(const RMatrix &);
        virtual SEXP get_SEXP() const throw() { return Rmat; }
        
        SEXP    Rmat;
        T      *data;
        Column *mcol;
        
        inline void get_dims()
        {
            SEXP      Rdim  = getAttrib(Rmat, R_DimSymbol);
            (size_t &)rows  = INTEGER(Rdim)[0];
            (size_t &)cols  = INTEGER(Rdim)[1];
            (size_t &)items = rows * cols;
            mcol = static_cast<Column *>( operator new( cols*sizeof(Column) ) );
            T *p = data;
            for( size_t i=0; i < cols; ++i, p += rows )
            {
                new (mcol+i) Column(p,rows);
            }
        }
    };
    
    //! C++ Matrix, rows major
    template <typename T>
    class CMatrix : public CoreMatrix
    {
    public:
        class Row
        {
        public:
            explicit Row( T *p, size_t c ) throw() :
            data(p),
            cols(c)
            {
                assert(data!=0);
                assert(cols>0);
            }
            
            inline T &       operator[](size_t c) throw()       { assert(c<cols); return data[c]; }
            inline const T & operator[](size_t c) const throw() { assert(c<cols); return data[c]; }
            
            
        private:
            Row(const Row &);
            Row&operator=(const Row &);
            ~Row();
            T *data;
        public:
            const size_t cols;
        };
        
        //! build a local matrix
        explicit CMatrix(size_t r, size_t c) :
        data(0),
        mrow(0)
        {
            build(r,c);
        }
        
        //! convert an R matrix to a C matrix
        explicit CMatrix( SEXP args ) :
        data(0),
        mrow(0)
        {
            //-- find R parameters
            SEXP Rmat = coerceVector(args, RGetData<T>::Conv);
            SEXP Rdim = getAttrib(Rmat, R_DimSymbol);
            const size_t r = INTEGER(Rdim)[0];
            const size_t c = INTEGER(Rdim)[1];
            const T     *p = RGetData<T>::Cast(Rmat);
            
            //-- prepare this matrix
            build(r,c);
            
            //-- copy items
            for(size_t i=0;i<rows;++i)
            {
                const size_t offset = i*cols;
                for(size_t j=0;j<cols;++j)
                {
                    data[offset+j] = p[i+rows*j];
                }
            }
        }
        
        virtual ~CMatrix() throw()
        {
            operator delete(mrow);
            delete []data;
            mrow = 0;
            data = 0;
        }
        
        inline Row &       operator[](size_t r) throw()       { assert(r<rows); return mrow[r]; }
        inline const Row & operator[](size_t r) const throw() { assert(r<rows); return mrow[r]; }
        
        
        inline void print() const
        {
            for(size_t i=0; i < rows; ++i )
            {
                for(size_t j=0;j<cols;++j)
                    Rprintf(" %g", (*this)[i][j]);
                Rprintf("\n");
            }
        }
        
    private:
        T   *data;
        Row *mrow;
        
        CMatrix( const CMatrix & );
        CMatrix&operator=(const CMatrix &);
        
        inline void build(size_t r,size_t c)
        {
            assert(r>0);
            assert(c>0);
            (size_t &)rows  = r;
            (size_t &)cols  = c;
            (size_t &)items = rows * cols;
            data = new T[items];
            try
            {
                mrow = static_cast<Row *>( operator new(rows*sizeof(Row)) );
                T *p = data;
                for(size_t i=0; i < rows; ++i, p += cols )
                {
                    new (mrow+i) Row(p,cols);
                }
            }
            catch(...)
            {
                delete []data;
                throw;
            }
        }
        
    };
    
    
    //! RList wrapper
    class RList : public RObject
    {
    public:
        
        //! create a list for R
        RList(const char *names[], const size_t count) :
        size( count ),
        L( allocVector(VECSXP,size) )
        {
            PROTECT(L);
            
            //-- set list items name
            SEXP list_names = 0;
            PROTECT(list_names = allocVector(STRSXP,size));
            for(size_t i = 0; i < size; i++)
                SET_STRING_ELT(list_names,i,mkChar(names[i]));
            setAttrib(L, R_NamesSymbol, list_names);
            UNPROTECT(1); //-- list_name
            
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
    };
    
    //! argument conversion
    template <typename T>
    inline T R2Scalar( SEXP r )
    {
        const RVector<T> value(r);
        if( value.size < 1) throw exception("R2Scalar: invalid argument");
        return value[0];
    }
    
    //! string conversion
    inline const char *R2String( SEXP r )
    {
        return CHAR(STRING_ELT(r,0));
    }

    
}

#endif
