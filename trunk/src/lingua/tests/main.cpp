#include "yocto/utest/driver.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_INIT(32)
{
    YOCTO_UNIT_TEST_DECL(token);
    YOCTO_UNIT_TEST_DECL(source);
    YOCTO_UNIT_TEST_DECL(basic);
    YOCTO_UNIT_TEST_DECL(logic);
    YOCTO_UNIT_TEST_DECL(joker);
    YOCTO_UNIT_TEST_DECL(posix);
    YOCTO_UNIT_TEST_DECL(dict);
    YOCTO_UNIT_TEST_DECL(viz);
    YOCTO_UNIT_TEST_DECL(rx);
    YOCTO_UNIT_TEST_DECL(scanner);
    YOCTO_UNIT_TEST_DECL(lexer);
    YOCTO_UNIT_TEST_DECL(syntax);
    YOCTO_UNIT_TEST_DECL(parser);
    YOCTO_UNIT_TEST_DECL(json);
    YOCTO_UNIT_TEST_DECL(cc);
    YOCTO_UNIT_TEST_DECL(match);
}
YOCTO_UNIT_TEST_EXEC()
