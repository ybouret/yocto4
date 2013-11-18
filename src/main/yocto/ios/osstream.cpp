#include "yocto/ios/osstream.hpp"

namespace yocto
{
	namespace ios
	{
		osstream:: osstream( string &host ) throw() : host_( host )
		{
		}
		
		osstream:: ~osstream() throw() {}
		
		
		void osstream:: write( char C ) { host_.append(C); }
		void osstream:: flush()  { }
		
	}
}
