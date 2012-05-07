
namespace yocto 
{
    namespace swamp
    {
        
        template <>
        array1D<ZTYPE>:: array1D( const layout_type &L ) throw() :
        linear_type(L),
        item(NULL)
        {
        }
        
        
        template <>
        array1D<ZTYPE>:: ~array1D() throw() {}
        
        template <>
        void array1D<ZTYPE>:: link( void *data ) throw()
        {
            assert( data != NULL );
            entry = (ZTYPE *)data;
            item  = entry - lower;
        }
        
        template <>
        array1D<ZTYPE>:: type & array1D<ZTYPE>:: operator[]( unit_t x ) throw()
        {
            assert(x>=lower);
            assert(x<=upper);
            assert(entry!=NULL);
            assert(item!=NULL);
            return item[x];
        }
        
        template <>
        array1D<ZTYPE>:: const_type & array1D<ZTYPE>:: operator[]( unit_t x ) const throw()
        {
            assert(x>=lower);
            assert(x<=upper);
            assert(entry!=NULL);
            assert(item!=NULL);
            return item[x];
        }
        
        template <>
        void array1D<ZTYPE>:: foreach( const layout_type &sub, callback proc, void *args )
        {
            assert( this->contains(sub) );
            assert( proc != NULL );
            for( unit_t x = sub.lower; x <= sub.upper; ++x)
            {
                proc( item[x], args );
            }
        }
        
        template <>
        void array1D<ZTYPE>:: foreach( const layout_type &sub, const_cb proc, void *args ) const
        {
            assert( this->contains(sub) );
            assert( proc != NULL );
            assert(item!=NULL);
            for( unit_t x = sub.lower; x <= sub.upper; ++x)
            {
                proc( item[x], args );
            }
        }
        
        template <>
        void array1D<ZTYPE>::  foreach( const array_type &other, const layout_type &sub, call_two proc, void *args)
        {
            assert( this->contains(sub) );
            assert( other.contains(sub) );
            assert(proc!=NULL);
            assert(item!=NULL);
            assert(other.item!=NULL);
            for( unit_t x = sub.lower; x <= sub.upper; ++x)
            {
                proc( item[x], other.item[x], args );
            }
        }
        
        template <>
        void array1D<ZTYPE>:: set( const array_type &other, const layout_type &sub ) throw()
        {
            foreach( other, sub, set2_cb, NULL );
        }
        
        template <>
        void array1D<ZTYPE>:: add( const array_type &other, const layout_type &sub ) throw()
        {
            foreach( other, sub, add2_cb, NULL );
        }
        
        template <>
		std::ostream & operator<<( std::ostream &os, const array1D<ZTYPE> &a )
		{
			os << '[';
			for( unit_t x=a.lower; x <= a.upper; ++x )
			{
				os << ' ' << a[x];
			}
			os << ' ' << ']';
			return os;
		}
        
        template <>
        void * array1D<ZTYPE>:: ctor( const layout_type &L, linear_base **info )
        {
            assert( info != NULL );
            assert( NULL == *info );
            array_type *arr = new array_type(L);
            *info = arr;
            return arr;
        }

        template <>
        void array1D<ZTYPE>:: dtor( void *handle ) throw()
        {
            assert( handle );
            array_type *a = (array_type *)handle;
            delete a;
        }
        
    }
}
