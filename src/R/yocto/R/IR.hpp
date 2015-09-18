#ifndef YOCTO_RMM_INCLUDED
#define YOCTO_RMM_INCLUDED 1

#include "yocto/code/bswap.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/sort/quick.hpp"
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

    //! Core Matrix
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

    private:
        CoreMatrix(const CoreMatrix &);
        CoreMatrix&operator=(const CoreMatrix&);
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    // CMatrix
    //
    ////////////////////////////////////////////////////////////////////////////

    //! C++ Matrix, rows major
    template <typename T>
    class CMatrix : public CoreMatrix
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
        mrow(0)
        {
            build(r,c);
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


    protected:
        T   *data;
    private:
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


}

#endif
