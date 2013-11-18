#include "yocto/utest/run.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/fs/working-directory.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;

static void ls( const string &dirname )
{
	local_fs              &fs  = local_fs::instance();
	auto_ptr<vfs::scanner> scan( fs.new_scanner( dirname ) );
	for( const vfs::entry *ep = scan->next(); ep; ep = scan->next() )
	{
		std::cerr << '<' << ep->attr << '>' << ep->base_name << std::endl;
	}
	std::cerr << "Done listing '" << dirname << "'" << std::endl;
}

YOCTO_UNIT_TEST_IMPL(wd)
{
	bool status = true;
	working_directory &wd = working_directory::instance();
	ls( wd.get() );
	std::cerr << std::endl;
	{
		chdir_lock work_in( ".." , &status);
		ls( wd.get() );
		std::cerr << std::endl;
	}
	ls( wd.get() );
	
}
YOCTO_UNIT_TEST_DONE()
