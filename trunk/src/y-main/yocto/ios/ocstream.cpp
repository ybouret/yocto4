#include "yocto/ios/ocstream.hpp"

namespace yocto
{

	namespace ios
	{

		ostream:: ~ostream() throw() {}

		ocstream:: ocstream( const char   *filename, bool append ) : cfile( filename, append ? "ab" : "wb" ) {}
		ocstream:: ocstream( const string &filename, bool append ) : cfile( filename.c_str(), append ? "ab" : "wb" ) {}
		ocstream:: ocstream( const cstdout_t &x ) : cfile(x) {}
		ocstream:: ocstream( const cstderr_t &x ) : cfile(x) {}

	}

}