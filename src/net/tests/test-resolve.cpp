#include "yocto/utest/run.hpp"
#include "yocto/net/net.hpp"
#include "yocto/net/ipaddr.hpp"

using namespace yocto;
using namespace network;


static inline void try_resolve( socket_address &ip, const string &name )
{
    net &network = net::instance();
    try
    {
        network.resolve(ip,name);
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << ip.fmt.version << " unhandled exception!" << std::endl;
    }
    std::cerr << ip.fmt.version << "=" << ip << std::endl;
}

YOCTO_UNIT_TEST_IMPL(resolve)
{
    std::cerr << "sizeof(IPv4)=" << sizeof(IPv4) << std::endl;
    std::cerr << "sizeof(IPv6)=" << sizeof(IPv6) << std::endl;

    for(int i=1;i<argc;++i)
    {
        const string name = argv[i];
        std::cerr << "resolving " << name << "..." << std::endl;

        IPv4 ipv4;
        try_resolve(ipv4,name);
        IPv6 ipv6;
        try_resolve(ipv6,name);

        //IPaddress ipaddr = new IPv4();
    }


}
YOCTO_UNIT_TEST_DONE()

