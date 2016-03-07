#include "yocto/program.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

YOCTO_PROGRAM_START()
{
    if(argc!=3)
    {
        throw exception("usage: %s source_file outdir", program);
    }

    const string  input_name = argv[1];
    ios::icstream inp(input_name);

    string       outdir     = argv[2];
    vfs::as_directory(outdir);
    string base_name  = vfs::get_base_name(input_name);
    vfs::remove_extension(base_name);
    const string header_name = outdir + base_name + ".hpp";
    const string source_name = outdir + base_name + ".cpp";

    


}
YOCTO_PROGRAM_END()

