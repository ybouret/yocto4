#include "yocto/program.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

YOCTO_PROGRAM_START()
{
    for(int i=1;i<argc;++i)
    {
        const string input_name = argv[i];
        string header_name = input_name;
        vfs::change_extension(header_name, "hpp");

        string source_name = input_name;
        vfs::change_extension(source_name, "cpp");

        std::cerr << "--> " << header_name << ", " << source_name << std::endl;

        string ramp_name = vfs::get_base_name(input_name);
        vfs::remove_extension(ramp_name);
        for(size_t j=0;j<ramp_name.length();++j)
        {
            char C = ramp_name[j];

            switch(C)
            {
                    case ' ':

                    C = '_';
                    break;

                default:
                    break;
            }
            ramp_name[j] = C;
        }
        std::cerr << "--> ramp = " << ramp_name << std::endl;

        string ramp_guard = "YOCTO_GFX_RAMP_" + ramp_name + "_INCLUDED";
        ramp_guard.to_upper();


        ios::ocstream header(header_name,false);
        ios::ocstream source(source_name,false);

        header << "#ifndef " << ramp_guard << "\n";
        header << "#define " << ramp_guard << " 1\n";
        header << "#include \"yocto/gfx/ramp.hpp\"\n";
        header << "\n";

        // declaration
        header << "namespace yocto { namespace gfx {\n";
        header << "\tclass " << ramp_name << " : public ramp {\n";
        header << "\tpublic:\n";
        header << "\t\tvirtual ~" << ramp_name << "() throw();\n";
        header << "\t\texplicit " << ramp_name << "() throw();\n";
        header << "\tprivate: YOCTO_DISABLE_COPY_AND_ASSIGN(" << ramp_name << ");\n";
        header << "\t}\n";

        header << "}}\n";
        header << "#endif\n";

        source << "#include \"./" << vfs::get_base_name(header_name) << "\"\n";
        source << "namespace yocto { namespace gfx {\n";
        source << "\tstatic const rgb_t __" << ramp_name << "[256]={\n";

        source << "\t};\n";
        source << "\t " << ramp_name << ":: " << ramp_name << "() throw() : ramp(__" << ramp_name << ") {}\n";
        source << "\t~" << ramp_name << ":: " << ramp_name << "() throw() {}\n";
        source << "}}\n";

    }

}
YOCTO_PROGRAM_END()