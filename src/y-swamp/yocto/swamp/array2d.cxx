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
            for( unit_t j = 0; j < width.y; ++j )
            {
                row[j].~row_type();
            }
            memory::kind<memory::global>::release_as<row_type>(row,num_rows);
        }
        
        template <>
        array2D<ZTYPE>::row_type & array2D<ZTYPE>:: operator[]( unit_t y ) throw()
        {
            assert( y >= lower.y );
            assert( y <= upper.y );
            return row[y];
        }
        
        template <>
        const array2D<ZTYPE>::row_type & array2D<ZTYPE>:: operator[]( unit_t y ) const throw()
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
        void array2D<ZTYPE>:: foreach( const array_type  &other, const layout_type &sub, call_two proc, void *args)
        {
            assert( this->contains(sub) );
            assert( other.contains(sub) );
            assert( proc != NULL );
            for( unit_t j=sub.lower.y; j <= sub.upper.y; ++j )
            {
                row_type       &self_j  = row[j];
                const row_type &other_j = other.row[j];
                for( unit_t i=sub.lower.x; i <= sub.upper.x; ++i )
                {
                    proc( self_j[i], other_j[i], args );
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
            entry = (ZTYPE *)data;
            ZTYPE *addr = entry;
            const size_t step = row_layout.items;
            for( unit_t j=lower.y; j <= upper.y; ++j, addr += step )
            {
                row[j].link( addr );
            }
            
        }
        
        template <>
        void * array2D<ZTYPE>:: ctor( const layout_type &L, linear_base **info )
        {
            assert( info != NULL );
            assert( NULL == *info );
            array_type *arr = new array_type(L);
            *info = arr;
            return arr;
        }
        
        template <>
        void array2D<ZTYPE>:: dtor( void *handle ) throw()
        {
            assert( handle );
            array_type *a = (array_type *)handle;
            delete a;
        }
        
        template <>
		void array2D<ZTYPE>:: ppm(const string   &filename,
                                  const string   &comment,
                                  const layout2D &area,
                                  double (*vproc)( const ZTYPE & ),
                                  const color::rgba32 *colors,
                                  double               vmin,
                                  double               vmax) const
		{
			assert( vproc != NULL );
			assert( this->has( area.lower ) );
			assert( this->has( area.upper ) );
			ios::ocstream fp( filename,false );
			fp("P6\n");
			
			//-- comment
			fp("#%s\n", &comment[0] );
			
			//-- size
			fp("%u %u\n", unsigned(area.width.x), unsigned(area.width.y) );
			
			//-- #colors
			fp("255\n");
			
			
			//-- data
			//-- #info
			const bool default_ramp = (NULL == colors);
			for( unit_t y = area.upper.y; y >= area.lower.y; --y )
			{
				const row_type &r_y = (*this)[y];
				for( unit_t x = area.lower.x; x <= area.upper.x; ++x )
				{
					const double v = vproc( r_y[x] );
                    //std::cerr << "v[" << y << "][" << x << "]=" << v << std::endl;
					if( default_ramp )
					{
						const color::rgba<double> c = color::rgba<double>::ramp( v, vmin, vmax );
						const uint8_t b[4] =  { uint8_t( 255 * c.r ), uint8_t( 255 * c.g ), uint8_t(255 * c.b ), 0 };
                        //std::cerr << v << "/(" << vmin << ":" << vmax <<") -> " << c.r << "," << c.g << "," << c.b << " = " << int(b[0]) << "," << int(b[1]) << "," << int(b[2]) << std::endl;
						fp.write( b[0] );
						fp.write( b[1] );
						fp.write( b[2] );
					}
					else
					{
						const color::rgba32 c = color::rgba32:: ramp( colors, v, vmin, vmax );
						fp.write( c.r );
						fp.write( c.g );
						fp.write( c.b );
					}
					
				}
			}
			
			
		}
        
    }
    
}
