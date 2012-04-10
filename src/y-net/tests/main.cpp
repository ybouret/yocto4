#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(resolve);
	YOCTO_UNIT_TEST_DECL(tcp_server);
	YOCTO_UNIT_TEST_DECL(tcp_client);
	YOCTO_UNIT_TEST_DECL(udp_server);
	YOCTO_UNIT_TEST_DECL(udp_client);
	YOCTO_UNIT_TEST_DECL(tcp_server_multi);
	YOCTO_UNIT_TEST_DECL(ioQ);
	YOCTO_UNIT_TEST_DECL(protocol);
    YOCTO_UNIT_TEST_DECL(rope);
}
YOCTO_UNIT_TEST_EXEC()
