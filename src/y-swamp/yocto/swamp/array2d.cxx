namespace yocto 
{
    namespace swamp
    {
        
        template <>
        array2D<ZTYPE>:: array2D( const layout_type &L ) :
        linear_type(L),
        row_layout(lower.x,upper.x),
        num_rows(width.y),
        row( memory::kind<memory::global>::acquire_as<row_type>(num_rows) )
        {
            for( unit_t j=0; j <width.y; ++j )
                new (row+j) row_type(row_layout); // no throw
            row -= lower.y;
        }
        
        
        template <>
        array2D<ZTYPE>:: ~array2D() throw() 
        {
            row += lower.y;
            for( unit_t j = width.y; j>0; --j )
            {
                row[j].~row_type();
            }
            memory::kind<memory::global>::release_as<row_type>(row,num_rows);
        }
        
        template <>
        array2D<ZTYPE>::row_type & array2D<ZTYPE>:: operator[]( size_t y ) throw()
        {
            assert( y >= lower.y );
            assert( y <= upper.y );
            return row[y];
        }
        
        template <>
        const array2D<ZTYPE>::row_type & array2D<ZTYPE>:: operator[]( size_t y ) const throw()
        {
            assert( y >= lower.y );
            assert( y <= upper.y );
            return row[y];
        }
        
        template <>
        void array2D<ZTYPE>:: foreach( const layout_type &sub, callback proc, void *args )
        {
            assert( this->contains(sub) );
            assert( proc != NULL );
            for( unit_t j=sub.lower.y; j <= sub.upper.y; ++j )
            {
                row_type &r_j = row[j];
                for( unit_t i=sub.lower.x; i <= sub.upper.x; ++i )
                {
                    proc( r_j[i], args );
                }
            }
        }
        
        template <>
        void array2D<ZTYPE>:: foreach( const layout_type &sub, const_cb proc, void *args ) const
        {
            assert( this->contains(sub) );
            assert( proc != NULL );
            for( unit_t j=sub.lower.y; j <= sub.upper.y; ++j )
            {
                const row_type &r_j = row[j];
                for( unit_t i=sub.lower.x; i <= sub.upper.x; ++i )
                {
                    proc( r_j[i], args );
                }
            }
        }

        
        template <>
        void array2D<ZTYPE>:: set( const array_type &other, const layout_type &sub ) throw()
        {
            foreach( other, sub, set2_cb, NULL );
        }
        
        template <>
        void array2D<ZTYPE>:: add( const array_type &other, const layout_type &sub ) throw()
        {
            foreach( other, sub, add2_cb, NULL );
        }

        
        template <>
        void array2D<ZTYPE>:: link( void *data ) throw()
        {
            assert( data != NULL );
            ZTYPE       *addr = (ZTYPE *)data;
            const size_t step = row_layout.items;
            for( unit_t j=lower.y; j <= upper.y; ++j, addr += step )
            {
                row[j].link( addr );
            }
            
        }
        
    }
    
}
