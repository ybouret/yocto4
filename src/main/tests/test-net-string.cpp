#include "yocto/ios/net-string.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/ios/osstream.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(net_string)
{
    ios::icstream ifp( ios::cstdin );
    ios::ocstream ofp( ios::cstderr );

    string  line;
    string  Bdata;
    string  Cdata;
    ios::osstream B_out(Bdata);
    ios::osstream C_out(Cdata);

    while( line.clear(), ifp.gets(line) )
    {
        std::cerr << "'" << line << "'" << std::endl;
        ios::net_string::write (line,ofp);  ofp << "\n";
        ios::net_string::format(line,ofp);  ofp << "\n";
        ios::net_string::write (line,B_out);
        ios::net_string::format(line,C_out);
    }

    std::cerr << "Bdata=" << Bdata << std::endl;
    std::cerr << "Cdata=" << Cdata << std::endl;

    string tmp;
    ios::imstream bin(Bdata);
    while( ios::net_string::read(bin,tmp) )
    {
        std::cerr << "read '" << tmp << "'" << std::endl;
    }


}
YOCTO_UNIT_TEST_DONE()

