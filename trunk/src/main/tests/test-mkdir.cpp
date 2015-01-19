#include "yocto/fs/local-fs.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(mkdir)
{

    local_fs &fs = local_fs::instance();

    if(argc>1)
    {
        fs.create_sub_dir(argv[1]);
    }

}
YOCTO_UNIT_TEST_DONE()
