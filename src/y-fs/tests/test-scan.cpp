#include "yocto/utest/run.hpp"
#include "yocto/filesys/local-fs.hpp"
#include "yocto/auto-ptr.hpp"

using namespace yocto;
using namespace filesys;

YOCTO_UNIT_TEST_IMPL(scan)
{
	const string dir = argc > 1 ? argv[1] : ".";
	local_fs    &fs  = local_fs::instance();
	auto_ptr<vfs::scanner> scan( fs.new_scanner( dir ) );
	for( const vfs::entry *ep = scan->next(); ep ; ep = scan->next() )
	{
		std::cerr << '<' << ep->attr << '>' << ep->base_name << " [" << ep->path << "]" << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()

