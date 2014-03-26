#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(16)
{
    YOCTO_UNIT_TEST_DECL(comp_rx);
    YOCTO_UNIT_TEST_DECL(pattern);
    YOCTO_UNIT_TEST_DECL(posix);
    YOCTO_UNIT_TEST_DECL(source);

    YOCTO_UNIT_TEST_DECL(scanner);
    YOCTO_UNIT_TEST_DECL(lexer);
    YOCTO_UNIT_TEST_DECL(string);
    YOCTO_UNIT_TEST_DECL(parse);
    YOCTO_UNIT_TEST_DECL(json);
    YOCTO_UNIT_TEST_DECL(comp_ast);
}
YOCTO_UNIT_TEST_EXEC()

