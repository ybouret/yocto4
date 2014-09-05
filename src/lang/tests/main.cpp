#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
	YOCTO_UNIT_TEST_DECL(token);
    YOCTO_UNIT_TEST_DECL(source);
    YOCTO_UNIT_TEST_DECL(pattern);
    YOCTO_UNIT_TEST_DECL(dict);
    YOCTO_UNIT_TEST_DECL(viz);
    YOCTO_UNIT_TEST_DECL(compiler);
    YOCTO_UNIT_TEST_DECL(scanner);
    YOCTO_UNIT_TEST_DECL(lexer);
    YOCTO_UNIT_TEST_DECL(xnode);
    YOCTO_UNIT_TEST_DECL(grammar);
    YOCTO_UNIT_TEST_DECL(parser);
    
}
YOCTO_UNIT_TEST_EXEC()

