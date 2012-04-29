namespace yocto 
{
    namespace swamp
    {
        
        template <>
        array3D<ZTYPE>:: array3D( const layout_type &L ) :
        linear_type(L),
        slice_layout( coord2D(lower.x,lower.y), coord2D(upper.x,upper.y) ),
        num_slices(width.z),
        slice( memory::kind<memory::global>::acquire_as<slice_type>(num_slices) )
        {
            unit_t k=0;
            try
            {
                for( ; k <width.z; ++k )
                    new (slice+k) slice_type(slice_layout); // no throw
                slice -= lower.z;
            }
            catch(...)
            {
                while( k >= 0 )
                    slice[k].~slice_type();
                memory::kind<memory::global>::release_as<slice_type>(slice,num_slices);
            }
        }
        
        
        template <>
        array3D<ZTYPE>:: ~array3D() throw() 
        {
            slice += lower.z;
            for( unit_t k=0; k < width.z; ++k )
            {
                slice[k].~slice_type();
            }
            memory::kind<memory::global>::release_as<slice_type>(slice,num_slices);
        }
        
        template <>
        array3D<ZTYPE>::slice_type & array3D<ZTYPE>:: operator[]( unit_t z ) throw()
        {
            assert( z >= lower.z );
            assert( z <= upper.z );
            return slice[z];
        }
        
        template <>
        const array3D<ZTYPE>::slice_type & array3D<ZTYPE>:: operator[]( unit_t z ) const throw()
        {
            assert( z >= lower.z );
            assert( z <= upper.z );
            return slice[z];
        }
        
        
        template <>
        void array3D<ZTYPE>:: foreach( const layout_type &sub, callback proc, void *args )
        {
            assert( this->contains(sub) );
            assert( proc != NULL );
            for( unit_t k=sub.lower.z; k <= sub.upper.z; ++k )
            {
                slice_type &S = slice[k];
                for( unit_t j=sub.lower.y; j <= sub.upper.y; ++j )
                {
                    row_type &R = S[j];
                    for( unit_t i=sub.lower.x; i <= sub.upper.x; ++i )
                    {
                        proc( R[i], args );
                    }
                }
            }
        }
        
        template <>
        void array3D<ZTYPE>:: foreach( const layout_type &sub, const_cb proc, void *args ) const
        {
            assert( this->contains(sub) );
            assert( proc != NULL );
            for( unit_t k=sub.lower.z; k <= sub.upper.z; ++k )
            {
                const slice_type &S = slice[k];
                for( unit_t j=sub.lower.y; j <= sub.upper.y; ++j )
                {
                    const row_type &R = S[j];
                    for( unit_t i=sub.lower.x; i <= sub.upper.x; ++i )
                    {
                        proc( R[i], args );
                    }
                }
            }
        }

              
        template <>
        void array3D<ZTYPE>:: foreach( const array_type  &other, const layout_type &sub, call_two proc, void *args)
        {
            assert( this->contains(sub) );
            assert( other.contains(sub) );
            assert( proc != NULL );
            for( unit_t k=sub.lower.z; k <= sub.upper.z; ++k )
            {
                slice_type &S  = slice[k];
                const slice_type &So = other.slice[k];
                for( unit_t j=sub.lower.y; j <= sub.upper.y; ++j )
                {
                    row_type       &R  = S[j];
                    const row_type &Ro = So[j];
                    for( unit_t i=sub.lower.x; i <= sub.upper.x; ++i )
                    {
                        proc( R[i], Ro[i], args );
                    }
                }
            }
  
        }
        
        template <>
        void array3D<ZTYPE>:: set( const array_type &other, const layout_type &sub ) throw()
        {
            foreach( other, sub, set2_cb, NULL );
        }
        
        template <>
        void array3D<ZTYPE>:: add( const array_type &other, const layout_type &sub ) throw()
        {
            foreach( other, sub, add2_cb, NULL );
        }
        
        
        template <>
        void array3D<ZTYPE>:: link( void *data ) throw()
        {
            assert( data != NULL );
            entry = (ZTYPE *)data;
            ZTYPE *addr = entry;
            const size_t step = slice_layout.items;
            for( unit_t k=lower.z; k <= upper.z; ++k, addr += step )
            {
                slice[k].link( addr );
            }
            
        }
        
        template <>
        void * array3D<ZTYPE>:: ctor( const layout_type &L )
        {
            return new array_type(L);
        }

        template <>
        void array3D<ZTYPE>:: dtor( void *handle ) throw()
        {
            assert( handle );
            array_type *a = (array_type *)handle;
            delete a;
        }

        
    }
    
}
