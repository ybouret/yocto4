

#if !defined(yCH)
#error "yCH must be defined"
#endif


namespace yocto {

    using namespace memory;
    namespace core {


        template<>
        string_match<yCH>:: ~string_match() throw()
        {
            kind<pooled>::release_as<yCH>( x_, l_);
        }


        template <>
        string_match<yCH>:: string_match( const yCH x[], const size_t m ) :
        m_( m ),
        x_( NULL ),
        k_( NULL ),
        l_(0)
        {
            const size_t x_length = m_ * sizeof(yCH);
            const size_t k_offset = YOCTO_MEMALIGN( x_length );
            const size_t k_length = m_ * sizeof(ptrdiff_t);

            l_ = k_offset + k_length;
            uint8_t     *f_buffer = kind<pooled>::acquire_as<uint8_t>( l_ );
            x_ = (yCH   *)(void *)&f_buffer[0];
            k_ = (ptrdiff_t *)(void *)&f_buffer[k_offset];
            memcpy( x_, x, m * sizeof(yCH) );


            ptrdiff_t i = 0;
            ptrdiff_t j = k_[0] = -1;
            while (i < m_)
            {
                while (j > -1 && x_[i] != x_[j])
                    j = k_[j];
                ++i;
                ++j;
                if (x_[i] == x_[j])
                    k_[i] = k_[j];
                else
                    k_[i] = j;
            }
        }


        template <>
        void string_match<yCH>::find( bool (*on_find)(size_t, void *), void *args, const yCH y[], const size_t n, const size_t nmax)
        {
            assert( on_find );
            size_t       count = 0;
            const ptrdiff_t n_ = n;
            /* Searching */
            ptrdiff_t i =0,  j = 0;
            while (j < n_) {
                while (i > -1 && x_[i] != y[j])
                    i = k_[i];
                ++i;
                ++j;
                if ( i >= m_) {
                    const size_t res = (j-i);
                    if( ! on_find( res, args ) ) //-- check if user wants to return
                        return;
                    if( nmax>0 && count >= nmax )
                        return; //-- global retrun
                    i = k_[i];
                }
            }
        }
        
        
        
    } // core
    
} // yocto
