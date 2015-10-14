#ifndef YOCTO_ARRAYS_MGR_INCLUDED
#define YOCTO_ARRAYS_MGR_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"

namespace yocto
{
    //! interface to a set or arrays of same size
    /**
     
     */
    template <typename T, typename MEMORY_KIND>
    class arrays_mgr
    {
    public:
        typedef lw_array<T>                           array_type;   //!< interface
        typedef typename type_traits<T>::mutable_type mutable_type; //!< internal data type
        
        
        const size_t size; //!< common size
        
        virtual size_t num_arrays() const throw() = 0;
        
        
        inline array<T> & operator[](size_t ia) throw()
        {
            assert(ia<num_arrays());
            return *(static_cast<array_type *>(arrays_addr())+ia);
        }
        
        inline const array<T> & operator[](size_t ia) const throw()
        {
            assert(ia<num_arrays());
            return *(static_cast<array_type *>(arrays_addr())+ia);
        }
        
        
        
        //! allocate memory for all
        inline void allocate(size_t nvar)
        {
            if(nvar!=size)
            {
                // acquire new data
                size_t        new_count = nvar * num_arrays();
                mutable_type *new_xdata = memory::kind<MEMORY_KIND>:: template acquire_as<mutable_type>( new_count );
				
                // release old data
				memory::kind<MEMORY_KIND>:: template release_as<mutable_type>( xdata, count );
                
                // link all
                xdata = new_xdata;
                count = new_count;
                (size_t&)size = nvar;
                link();
            }
            else
            {
                char *p = (char *)xdata;
                for(size_t i=count*sizeof(T);i>0;--i) *(p++) = 0;
            }
        }
        
        //! release all memory
        inline void release() throw()
        {
            if(size)
            {
                memory::kind<MEMORY_KIND>:: template release_as<mutable_type>( xdata, count );
                (size_t&)size=0;
                link();
            }
        }

        
        virtual ~arrays_mgr() throw() { assert(size==0); }
        
        
        //! one shot helper
        inline array<T> & next_array() throw()
        {
            assert(indx<num_arrays());
            array_type *a = static_cast<array_type *>(arrays_addr());
            return a[indx++];
        }
        
        
    protected:
        explicit arrays_mgr() throw() :
        size(0),
        count(0),
        xdata(0),
        indx(0)
        {
        }
        
       
        
        inline void link() throw()
        {
            mutable_type *p = xdata;
            array_type   *a = static_cast<array_type *>(arrays_addr());
            for(size_t i=num_arrays();i>0;--i)
            {
                new (a++) array_type(p,size);
                p += size;
            }
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(arrays_mgr);
        size_t        count;
        mutable_type *xdata;
        size_t        indx;
        
        virtual void *arrays_addr() const throw() = 0;
        
    };
    
}

#endif

