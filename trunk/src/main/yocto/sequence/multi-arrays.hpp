#ifndef YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED
#define YOCTO_SEQUENCE_MULTI_ARRAYS_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto
{
    
    template <
    size_t   N,
    typename T,
    typename MEMORY_KIND
    >
    class multi_arrays
    {
    public:
        static const size_t ARRAYS = N;
        typedef          lw_array<T>                  array_type;
        typedef typename type_traits<T>::mutable_type mutable_type;
        
        //! build N empty arrays
        explicit multi_arrays() throw() :
        size(0), buflen(0), buffer(0), wksp(), indx(0)
        {
            YOCTO_STATIC_CHECK(N>0,invalid_count);
            link();
        }
        
        //! build N arrays with same size=nvar
        explicit multi_arrays(size_t nvar) :
        size(0), buflen(0), buffer(0), wksp(), indx(0)
        {
            YOCTO_STATIC_CHECK(N>0,invalid_count);
            prepare(nvar);
        }
        
        //! release all memory
        virtual ~multi_arrays() throw() { release(); }
        
        
        //! release all memory
        inline void release() throw()
        {
            if(size)
            {
                memory::kind<MEMORY_KIND>:: template release_as<mutable_type>( buffer, buflen );
                (size_t&)size=0;
                link();
            }
        }
        
        
        //! clean all
        inline void prepare(size_t nvar)
        {
            if( nvar != size )
            {
                // allocate new data
                size_t        new_len = N * sizeof(T);
                mutable_type *new_buf = memory::kind<MEMORY_KIND>:: template acquire_as<mutable_type>(new_len);
                
                // release old data
                memory::kind<MEMORY_KIND>:: template release_as<mutable_type>( buffer, buflen );
                buffer        = new_buf;
                buflen        = new_len;
                
                // link
                (size_t&)size = nvar;
                link();
            }
            indx = 0;
        }
        
        
        //! get the array
        inline array<T> & operator[](size_t ia) throw()
        {
            assert(ia<N);
            array_type *a = (array_type *) (&wksp[0]);
            return a[ia];
        }
        
        
        //! get the array
        inline const array<T> & operator[](size_t ia) const throw()
        {
            assert(ia<N);
            const array_type *a = (array_type *) (&wksp[0]);
            return a[ia];
        }

        //! for help
        inline array<T> & next_array() throw()
        {
            assert(indx<N);
            array_type *a = (array_type *) (&wksp[0]);
            return a[indx++];
        }
        
        
        //! for info
        inline size_t arrays() const throw() { return ARRAYS; }
        
        
        const size_t size;  //!< common size

    private:
        size_t        buflen; // N * size
        mutable_type *buffer; // allocated memeory
        uint64_t      wksp[ YOCTO_U64_FOR_SIZE(N*sizeof(array_type)) ];
        size_t        indx; //!< free array index
        
        inline void link() throw()
        {
            //for(size_t i=0;i<sizeof(wksp)/sizeof(wksp[0]);++i) wksp[i] = 0;
            mutable_type *p = buffer;
            array_type   *a = (array_type *) (&wksp[0]);
            for(size_t i=0;i<N;++i)
            {
                new (a+i) array_type(p,size);
                p += size;
            }
        }
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(multi_arrays);
    };
    
}

#endif

