#include "yocto/utest/run.hpp"
#include "yocto/net/net.hpp"

using namespace yocto;
using namespace network;

YOCTO_UNIT_TEST_IMPL(get_host_name)
{
    net &Net = net::instance();
    const string hostname = Net.get_host_name();
    std::cerr << "hostname=" << hostname << std::endl;
}
YOCTO_UNIT_TEST_DONE()

