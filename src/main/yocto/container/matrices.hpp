#ifndef YOCTO_MATH_MATRICES_INCLUDED
#define YOCTO_MATH_MATRICES_INCLUDED 1

#include "yocto/container/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{

    //! interface to interfaces...
    template <typename T>
    class matrices_of
    {
    public:
        virtual ~matrices_of() throw() {}

        inline size_t size() const throw() { return handles.size(); }

        inline matrix_of<T> & operator[](const size_t i) throw()
        {
            assert(i>=1);
            assert(i<=handles.size());
            return *handles[i];
        }

        inline const matrix_of<T> & operator[](const size_t i) const throw()
        {
            assert(i>=1);
            assert(i<=handles.size());
            return *handles[i];
        }

        inline void ldz()
        {
            for(size_t i=1;i<=count;++i)
            {
                handles[i]->ldz();
            }
        }

        inline void neg()
        {
            for(size_t i=1;i<=count;++i)
            {
                handles[i]->neg();
            }
        }


    protected:
        inline matrices_of(size_t nm, size_t nr, size_t nc) :
        handles(nm,as_capacity),
        count(nm),
        rows(nr),
        cols(nc)
        {

        }

        typedef shared_ptr< matrix_of<T> > matptr;
        vector<matptr> handles;

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(matrices_of);

    public:
        const size_t count;
        const size_t rows;
        const size_t cols;
    };


    template <
    typename T,
    template <typename> class MATRIX>
    class matrices : public matrices_of<T>
    {
    public:
        typedef MATRIX<T> matrix_type;
        typedef typename  matrices_of<T>::matptr matptr;

        virtual ~matrices() throw() {}

        inline explicit matrices(size_t nm,size_t nr,size_t nc) :
        matrices_of<T>(nm,nr,nc),
        impl(nm,as_capacity)
        {
            for(size_t i=nm;i>0;--i)
            {
                matrix_type *q = new matrix_type(nr,nc);
                const matptr p(q);
                this->handles.push_back(p);
                impl.push_back(q);
            }
            assert(this->handles.size()==this->count);
        }

        inline matrix_type & operator()(const size_t im) throw()
        {
            assert(im>=1);
            assert(im<=impl.size());
            return *impl[im];
        }

        inline const matrix_type & operator()(const size_t im) const throw()
        {
            assert(im>=1);
            assert(im<=impl.size());
            return *impl[im];
        }


    private:
        vector<matrix_type*> impl;
        YOCTO_DISABLE_COPY_AND_ASSIGN(matrices);
    };
    
}


#endif
