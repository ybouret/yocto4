#include "yocto/program.hpp"
#include "yocto/lingua/parser.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/compound.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/hashing/mph.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_PROGRAM_START()
{
    if(argc<=2)
    {
        throw exception("usage: %s PREFIX grammar_file [output_file]", program);
    }

    const string            prefix   = argv[1];
    const string            gramfile = argv[2];
    ios::icstream           inp(gramfile);
    auto_ptr<parser>        P( parser::generate(inp) );
    const syntax::grammar  &G = *P;
    vector<string>          Terms;
    vector<string>          Rules;
    (void)G.collect(Terms,Rules);

    auto_ptr<ios::ostream> pfp( new ios::ocstream( ios::cstdout ) );
    if(argc>3)             pfp.reset( new ios::wcstream(argv[3])  );
    ios::ostream &fp = *pfp;
    fp << "// TERMINAL\n";
    hashing::mperf::emit_defines(fp,Terms,prefix,0);
    fp << "// INTERNAL\n";
    hashing::mperf::emit_defines(fp,Rules,prefix,Terms.size());

}
YOCTO_PROGRAM_END()
