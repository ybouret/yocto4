#include "yocto/codec/codec.hpp"


namespace yocto
{
	
	namespace codec
	{
		
		filter:: ~filter() throw() {}
		
		filter:: filter( const writer &w ) : output(w) {}
		
		
	}
}