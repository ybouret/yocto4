#ifndef YOCTO_MATH_MATRICES_INCLUDED
#define YOCTO_MATH_MATRICES_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{

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
        virtual ~matrices() throw() {}

        inline explicit matrices(size_t nm,size_t nr,size_t nc) :
        matrices_of<T>(nm,nr,nc)
        {
            for(size_t i=nm;i>0;--i)
            {
                this->handles.template append<matrix_of<T> *>( new MATRIX<T>(nr,nc) );
            }
            assert(this->handles.size()==this->count);
        }
        

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(matrices);
    };
    
}


#endif
