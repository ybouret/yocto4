#include "yocto/utest/run.hpp"
#include "yocto/z++/gzstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/z++/zpipe.hpp"

#include <cstdlib>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(iz)
{
	std::cerr << "reading file" << std::endl;
    if( argc > 1)
    {
        const string file_name = argv[1];
		ios::igzstream   fp( file_name );
		ios::ocstream    out( "iz.out", false);

        string line;
        while( fp.read_line( line ) >= 0)
        {
			out << line << '\n';
            line.clear();
        }
    }

}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(oz)
{
	std::cerr << "writing foo.gz" << std::endl;
	ios::ogzstream fp( "foo.gz", 7 );
	ios::icstream in( ios::cstdin );

	string line;
	while( in.read_line( line ) >= 0 && line != ".quit" )
	{
		//std::cerr << "->" << line << std::endl;
		fp << line << '\n';
		line.clear();
	}

}
YOCTO_UNIT_TEST_DONE()

#include "yocto/z++/zpipe.hpp"

YOCTO_UNIT_TEST_IMPL(def)
{
	size_t level = argc > 1 ? atol( argv[1]  ) : 7;
	size_t chunk = argc > 2 ? atol( argv[2]  ) : 16 * 1024;

	ios::icstream source( ios::cstdin );
	ios::ocstream target( ios::cstdout );

	zlib::zpipe zp( chunk );
    memory::no_blender blend;
	zp.def( target, source, level, blend );



}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(inf)
{
	size_t chunk = argc > 1 ? atol( argv[1]  ) : 16 * 1024;

	ios::icstream source( ios::cstdin );
	ios::ocstream target( ios::cstdout );

	zlib::zpipe zp( chunk );
    memory::no_blender blend;

	zp.inf( target, source, blend);



}
YOCTO_UNIT_TEST_DONE()


