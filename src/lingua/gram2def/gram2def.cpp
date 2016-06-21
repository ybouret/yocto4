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
    if(argc<=1)
    {
        throw exception("usage: %s grammar_file [output_file]", program);
    }

    const string            gramfile = argv[1];
    ios::icstream           inp(gramfile);
    auto_ptr<parser>        P( parser::generate(inp) );
    const syntax::grammar  &G = *P;
    vector<string>          Terms;
    vector<string>          Rules;
    (void)G.collect(Terms,Rules);

    auto_ptr<ios::ostream> pfp( new ios::ocstream( ios::cstdout ) );
    if(argc>2)             pfp.reset( new ios::wcstream(argv[2])  );
    ios::ostream &fp = *pfp;
    const string pfx;
    fp << "// TERMINAL\n";
    hashing::mperf::emit_defines(fp,Terms,pfx,0);
    fp << "// INTERNAL\n";
    hashing::mperf::emit_defines(fp,Rules,pfx,Terms.size());

}
YOCTO_PROGRAM_END()
