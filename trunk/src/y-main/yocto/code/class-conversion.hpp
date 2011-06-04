#ifndef YOCTO_CLASS_CONVERSION
#define YOCTO_CLASS_CONVERSION 1

#include "yocto/os.hpp"

namespace yocto 
{
	template <class T,class U>
	class conversion {
	private:
		typedef uint8_t small;
		class           large { uint32_t unused; };
		
		static  small test( U );
		static  large test(...);
		static  T     makeT();
		
	public:
		conversion(); //!< or some compiler will complain about all private...
		enum
		{
			exists   = ( sizeof( test(makeT()) ) == sizeof(small) ),
			same_type = false
		};
	};
	
	template <class T >
	class conversion<T,T> {
	public:
		enum {
			exists   = true,
			same_type = true
		};
	};
	
#define YOCTO_IS_SUPERSUBCLASS(T,U) \
(conversion<const U *,const T *>::exists && (!conversion<const T *, const void *>::same_type) )
	
#define YOCTO_IS_SUPERSUBCLASS_STRICT(T,U) \
( YOCTO_IS_SUPERSUBCLASS(T,U) && (!conversion<const T,const U>::same_type) )
	
}


#endif
