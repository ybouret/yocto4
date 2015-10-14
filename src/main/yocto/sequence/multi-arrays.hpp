#ifndef YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED
#define YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    
    template <typename T>
    class arrays_manager
    {
    public:
        typedef T                                     type;
        typedef typename type_traits<T>::mutable_type mutable_type; //!< internal data type
        
        
        virtual array<T>       &operator[](size_t ia) throw()       = 0;
        virtual const array<T> &operator[](size_t ia) const throw() = 0;
        virtual size_t          num_arrays() const throw()          = 0;
        virtual void allocate(size_t nvar) = 0;
        virtual void release(void) throw() = 0;
        
        const size_t size; //! current size for all arrays
        
        virtual ~arrays_manager() throw() { assert(0==size); }

    protected:
        explicit arrays_manager() throw() : size(0) {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(arrays_manager);
    };
    
    //! interface to a handle some arrays of same sizes
    /**
     The type must accept a zero argument constructor
     */
    template<
    typename T,
    typename MEMORY_KIND
    >
    class multi_arrays : public arrays_manager<T>
    {
    public:
        typedef lw_array<T>                                          array_type;   //!< interface
        typedef typename arrays_manager<T>::type                     type;
        typedef typename arrays_manager<T>::mutable_type             mutable_type;
        typedef vector<mutable_type,typename MEMORY_KIND::allocator> inmem_type;
        
        
        virtual array<T> & operator[](size_t ia) throw()
        {
            assert(ia<this->num_arrays());
            return *(static_cast<array_type *>(arrays_addr())+ia);
        }
        
        virtual const array<T> & operator[](size_t ia) const throw()
        {
            assert(ia<this->num_arrays());
            return *(static_cast<array_type *>(arrays_addr())+ia);
        }
        
        
        //! allocate memory for all
        virtual void allocate(size_t nvar)
        {

            if(nvar!=this->size)
            {
                inmem.make(nvar*this->num_arrays());
                (size_t&)(this->size) = nvar;
                link();
            }
        }
        
        //! release all memory
        virtual void release() throw()
        {
            if(this->size)
            {
                inmem.release();
                (size_t&)(this->size)=0;
                link();
            }
        }
        
        
        virtual ~multi_arrays() throw() {}
        
        
        //! one shot helper, using internal indx
        inline array<T> & next_array() throw()
        {
            assert(indx<this->num_arrays());
            array_type *a = static_cast<array_type *>(arrays_addr());
            return a[indx++];
        }
        
        
    protected:
        explicit multi_arrays() throw() :
        inmem(),
        indx(0)
        {
        }
        
        
        
        inline void link() throw()
        {
            mutable_type *p = inmem(0);
            array_type   *a = static_cast<array_type *>(arrays_addr());
            for(size_t i=this->num_arrays();i>0;--i)
            {
                new (a++) array_type(p,this->size);
                p += this->size;
            }
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(multi_arrays);
        inmem_type    inmem;
        size_t        indx;

        virtual void *arrays_addr() const throw() = 0;
        
    };
    
}


#endif

