#ifndef YOCTO_RMM_INCLUDED
#define YOCTO_RMM_INCLUDED 1

#include "yocto/code/bswap.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/type/traits.hpp"

#include <new>

namespace yocto
{

    ////////////////////////////////////////////////////////////////////////////
    //
    // RArray interface
    //
    ////////////////////////////////////////////////////////////////////////////

#define YOCTO_R_ARRAY_IMPL(ADDR,ITEMS) \
inline virtual size_t size() const throw() { return ITEMS; } \
inline virtual T       &operator[](const size_t indx) throw()       { assert(indx<ITEMS); return ADDR[indx]; } \
inline virtual const T &operator[](const size_t indx) const throw() { assert(indx<ITEMS); return ADDR[indx]; }

    //! interface for a linear array
    template <typename T>
    class RArray
    {
    public:

        virtual ~RArray() throw() {}
        virtual  size_t size() const throw() = 0;
        virtual  T       &operator[](const size_t indx) throw() = 0;
        virtual  const T &operator[](const size_t indx) const throw() = 0;

    protected:
        inline explicit RArray() throw() {}

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(RArray);
    };

    template <typename T,typename FUNC>
    inline void RQSort( RArray<T> &arr, FUNC &cmp )
    {
        const size_t na = arr.size();
        if(na>1)
        {
            lw_array<T> a( &arr[0], na );
            quicksort(a,cmp);
        }
    }

    template <typename T>
    inline void RQSort( RArray<T> &arr )
    {
        RQSort(arr,__compare<T>);
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // Array based on user's data
    //
    ////////////////////////////////////////////////////////////////////////////
    template <typename T>
    class LArray : public RArray<T>
    {
    public:
        inline explicit LArray( T *usr_data, const size_t usr_size) throw() :
        data(usr_data),
        items(usr_size)
        {
            assert(!(data==NULL&&items>0));
        }

        inline virtual ~LArray() throw() {}

        YOCTO_R_ARRAY_IMPL(data,items)
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(LArray);
        T           *data;
        const size_t items;
    };


    ////////////////////////////////////////////////////////////////////////////
    //
    // CVector
    //
    ////////////////////////////////////////////////////////////////////////////

#define YOCTO_R_CVECTOR_CTOR() \
inMem(items),\
data(memory::kind<memory::global>::acquire_as<T>(inMem))

    template <typename T>
    class CVector : public RArray<T>
    {
    public:
        explicit CVector(size_t n) :
        items(n),
        YOCTO_R_CVECTOR_CTOR()
        {
        }

        virtual ~CVector() throw()
        {
            memory::kind<memory::global>::release_as<T>(data,inMem);
        }


        inline void swap_with(CVector &other) throw()
        {
            cswap_const(items,other.items);
            cswap(inMem,other.inMem);
            cswap(data,other.data);
        }

        inline CVector(const RArray<T> &arr) :
        items(arr.size()),
        YOCTO_R_CVECTOR_CTOR()
        {
#define YOCTO_R_CVECTOR_COPY(INDX) data[INDX] = arr[INDX]
            YOCTO_LOOP_FUNC(items,YOCTO_R_CVECTOR_COPY,0);
        }

        inline CVector & operator=( const RArray<T> &arr )
        {
            if(arr.size()==items)
            {
                YOCTO_LOOP_FUNC(items,YOCTO_R_CVECTOR_COPY,0);
#undef YOCTO_R_CVECTOR_COPY
            }
            else
            {
                CVector tmp(arr);
                swap_with(tmp);
            }
            return *this;
        }

        YOCTO_R_ARRAY_IMPL(data,items)

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(CVector);
        const size_t items;
        size_t       inMem;
        T           *data;
    };


    ////////////////////////////////////////////////////////////////////////////
    //
    // CoreMatrix
    //
    ////////////////////////////////////////////////////////////////////////////

    //!
    class CoreMatrix
    {
    public:
        const size_t rows;
        const size_t cols;
        const size_t items;

        inline virtual ~CoreMatrix() throw()
        {
            (size_t&)rows  = 0;
            (size_t&)cols  = 0;
            (size_t&)items = 0;
        }


    protected:
        inline explicit CoreMatrix() throw() :
        rows(0), cols(0), items(0)
        {}

        inline void setDimensions(const size_t nr, const size_t nc) throw()
        {
            assert(nr>0);
            assert(nc>0);
            (size_t &)rows  = nr;
            (size_t &)cols  = nc;
            (size_t &)items = nr*nc;
        }

    private:
        CoreMatrix(const CoreMatrix &);
        CoreMatrix&operator=(const CoreMatrix&);
    };

    template <typename T>
    class Matrix : public CoreMatrix
    {
    public:
        inline virtual ~Matrix() throw() {}

        virtual T &       operator()(size_t ir, size_t ic) throw()       = 0;
        virtual const T & operator()(size_t ir, size_t ic) const throw() = 0;

        inline friend std::ostream & operator<<( std::ostream &os, const Matrix &M )
        {
            assert(M.rows>0);
            assert(M.cols>0);
            const size_t rm1 = M.rows-1;
            os << '[';
            for(size_t i=0;i<M.rows;++i)
            {
                for(size_t j=0;j<M.cols;++j)
                {
                    os << ' ' << M(i,j);
                }
                if(i<rm1) os << ';';
            }
            os << ']';
            return os;
        }

        inline void ld( typename type_traits<T>::parameter_type arg )
        {
            Matrix<T> &self = *this;
            for(size_t r=0;r<rows;++r)
            {
                for(size_t c=0;c<cols;++c)
                {
                    self(r,c) = arg;
                }
            }
        }
        
        inline void neg()
        {
            Matrix<T> &self = *this;
            for(size_t r=0;r<rows;++r)
            {
                for(size_t c=0;c<cols;++c)
                {
                    const T   tmp = -self(r,c);
                    self(r,c) = tmp;
                }
            }

        }
        

    protected:
        inline explicit Matrix() throw() : CoreMatrix() {}

    private:
        Matrix(const Matrix &);
        Matrix&operator=(const Matrix&);
    };


    ////////////////////////////////////////////////////////////////////////////
    //
    // CMatrix
    //
    ////////////////////////////////////////////////////////////////////////////

    //! C++ Matrix, rows major
    template <typename T>
    class CMatrix : public Matrix<T>
    {
    public:

        class Row : public RArray<T>
        {
        public:
            inline explicit Row( T *p, size_t c ) throw() :
            data(p),
            cols(c)
            {
                assert(data!=0);
                assert(cols>0);
            }

            YOCTO_R_ARRAY_IMPL(data,cols)
        private:
            Row(const Row &);
            Row&operator=(const Row &);
            virtual ~Row() throw() {}
            T *data;
        public:
            const size_t cols;
        };

        //! build a local matrix
        explicit CMatrix(size_t r, size_t c) :
        data(0),
        mrow(0),
        wlen(0),
        done(0)
        {
            build(r,c);
        }


        virtual ~CMatrix() throw()
        {
            release();
        }

        inline Row &       operator[](size_t r) throw()       { assert(r<this->rows); return mrow[r]; }
        inline const Row & operator[](size_t r) const throw() { assert(r<this->rows); return mrow[r]; }

        inline virtual T & operator()(size_t ir, size_t ic) throw()
        {
            return (*this)[ir][ic];
        }

        inline virtual const T & operator()(size_t ir, size_t ic) const throw()
        {
            return (*this)[ir][ic];
        }

    protected:
        T     *data;
    private:
        Row   *mrow;
        size_t wlen;
        size_t done;

        CMatrix( const CMatrix & );
        CMatrix&operator=(const CMatrix &);

        // kill primitive type
        inline void kill( int2type<true> ) throw()
        {
            return;
        }

        // kill object type
        inline void kill( int2type<false> ) throw()
        {
            while(done>0)
            {
                destruct(&data[--done]);
            }
        }

        inline void cleanUp() throw() { kill( int2type< type_traits<T>::is_primitive >() ); }

        void release() throw()
        {
            cleanUp();
            void *base = (void *)mrow;
            memory::kind<memory::global>::release(base,wlen);
            mrow = 0;
            data = 0;
        }

        inline void build(size_t r,size_t c)
        {
            //__________________________________________________________________
            //
            // compute metrics
            //__________________________________________________________________
            assert(r>0);
            assert(c>0);
            this->setDimensions(r,c);

            //__________________________________________________________________
            //
            // get memory
            //__________________________________________________________________
            const size_t row_bytes  = r*sizeof(Row);
            const size_t data_base  = memory::align(row_bytes);
            wlen = data_base + this->items * sizeof(T);
            void *base = memory::kind<memory::global>::acquire(wlen);

            //__________________________________________________________________
            //
            // prepare memory layout
            //__________________________________________________________________
            mrow = static_cast<Row *>(base);
            data = (T *) (static_cast<uint8_t*>(base)+data_base);
            {
                T *p = data;
                for(size_t i=0;i<r;++i,p+=c)
                {
                    new (mrow+i) Row(p,c);
                }
            }

            //__________________________________________________________________
            //
            // prepare objects
            //__________________________________________________________________
            try
            {
                init( int2type< type_traits<T>::is_primitive >() );
            }
            catch(...)
            {
                release();
                throw;
            }
        }

        //! primitive
        inline void init( int2type<true> ) throw()
        {
            //do nothing, initialized to 0
        }
        
        //! primitive
        inline void init( int2type<false> )
        {
            assert(0==done);
            const size_t n = this->items;
            while(done<n)
            {
                new (data+done) T();
                ++done;
            }
        }
        
    };
    
    
}

#endif
