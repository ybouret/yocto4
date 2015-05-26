#include "yocto/utest/run.hpp"
#include "yocto/net/ipaddr.hpp"
#include "yocto/net/socket/set.hpp"
#include "yocto/net/socket/tcp.hpp"
#include "yocto/memory/buffers.hpp"

#include <cstdlib>

using namespace yocto;
using namespace network;


YOCTO_UNIT_TEST_IMPL(tcp_multi)
{
    std::cerr << "socket_set::max_size=" << socket_set::max_size << std::endl;
}
YOCTO_UNIT_TEST_DONE()

