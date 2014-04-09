#include "yocto/utest/run.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;

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

YOCTO_UNIT_TEST_IMPL(file_size)
{
    local_fs    &fs  = local_fs::instance();
    for(int i=1;i<argc;++i)
    {
        try
        {
            const string fn = argv[i];
            std::cerr << "file_size(" << fn << ")=";
            std::cerr.flush();
            std::cerr << fs.get_file_size(fn);
            std::cerr << std::endl;
        }
        catch(const exception &e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << e.when() << std::endl;
        }
    }
}
YOCTO_UNIT_TEST_DONE()
