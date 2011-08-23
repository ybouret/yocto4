#include "yocto/utest/driver.hpp"

YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(source);
	YOCTO_UNIT_TEST_DECL(pattern);
	YOCTO_UNIT_TEST_DECL(posix);
	YOCTO_UNIT_TEST_DECL(compiler);
	YOCTO_UNIT_TEST_DECL(lexer);
}
YOCTO_UNIT_TEST_EXEC()
