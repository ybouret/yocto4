#include "yocto/math/sig/data-extender.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exception.hpp"

namespace yocto {
	
	namespace math {
		
		template <>
		extender<real_t>:: ~extender() throw()
		{
			
		}
		
		template <>
		extender<real_t>:: extender(const array<real_t>    &data,
									const extension_type    dxlo,
									const extension_type    dxup ) :
		data_( data         ),
		size_( data_.size() ),
		dxlo_( dxlo ),
		dxup_( dxup ),
		tdlo_( REAL(2.0) * data_[1]      ),
		tdup_( REAL(2.0) * data_[size_]  )
		{
			if( size_ <= 0 )
				throw exception( "[extender] overflow ith size=%u", unsigned(data_.size()));
		}
		
		
		template <>
		real_t extender<real_t>::operator[]( ptrdiff_t i ) const throw()
		{
			if( i < 1 )
			{
				//--------------------------------------------------------------
				//
				// lower boundary conditions
				//
				//--------------------------------------------------------------
				switch( dxlo_ )
				{
					case extension_constant:
						return data_[1];
						
					case extension_cyclic:
						return (*this)[i+size_];
						
					case extension_reflect:
						return (*this)[1-i];
						
					case extension_symmetric:
						return tdlo_ - (*this)[1-i];
						
				}
				
			}
			else
				if( i > size_ )
				{
					//----------------------------------------------------------
					//
					// upper boundary conditions
					//
					//----------------------------------------------------------
					
					switch( dxup_ )
					{
						case extension_constant:
							return data_[size_];
							
						case extension_cyclic:
							return (*this)[i-size_];
							
						case extension_reflect:
							return (*this)[size_-(i-size_)];
							
						case extension_symmetric:
							return tdup_ - (*this)[size_-(i-size_)];
					}
					
				}
			//------------------------------------------------------------------
			//
			// default
			//
			//------------------------------------------------------------------
			return data_[i];
			
		}
		
		
	} // math
	
} //syquo
